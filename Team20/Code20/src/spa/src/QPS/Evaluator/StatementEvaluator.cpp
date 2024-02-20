#include "StatementEvaluator.h"
#include "ClauseEvaluator.h"


namespace QueryEvaluator {

    bool StatementEvaluator::evaluate() {
        // TODO: handles clauses with only stmtRef

        PQL::Synonym lArg = clause.arguments[0];
        PQL::Synonym rArg = clause.arguments[1];

        // TODO: HANDLES (STMT_NO, STMT_NO), (WILDCARD, WILDCARD), (SYN, SYN)
        if (lArg.entityType == rArg.entityType) {
            if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                int leftStmtNum = stoi(lArg.identity);
                int rightStmtNum = stoi(rArg.identity);
                if (leftStmtNum == rightStmtNum) {
                    return false;
                }

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    return reader->containsFollowsRelationship(leftStmtNum, rightStmtNum);
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    return reader->containsFollowsTRelationship(leftStmtNum, rightStmtNum);
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    return reader->containsParentRelationship(leftStmtNum, rightStmtNum);
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    return reader->containsParentTRelationship(leftStmtNum, rightStmtNum);
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    return reader->hasFollowsRelationship();
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    return reader->hasFollowsTRelationship();
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    return reader->hasParentRelationship();
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    return reader->hasParentTRelationship();
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (lArg.entityType == SimpleProgram::DesignEntity::STMT
                    || lArg.entityType == SimpleProgram::DesignEntity::READ
                    || lArg.entityType == SimpleProgram::DesignEntity::PRINT
                    || lArg.entityType == SimpleProgram::DesignEntity::ASSIGN
                    || lArg.entityType == SimpleProgram::DesignEntity::CALL
                    || lArg.entityType == SimpleProgram::DesignEntity::WHILE
                    || lArg.entityType == SimpleProgram::DesignEntity::IF) {
                // e.g. follows(s, s)
                return false;
            } else {
                // TODO: throw illegal argument, not allowed entity type for statRef
                return false;
            }
        }

        // TODO: HANDLES (STMT SYN, STMT_NUM), (STMT SYN, WILDCARD), (STMT SYN, SYN)
        if (lArg.entityType == SimpleProgram::DesignEntity::STMT) {
            std::vector<int> stmtNums = reader->getAllStatementNum();
            std::vector<std::string> lResults = {};
            std::vector<std::string> rResults = {};

            if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                int stmtNum = stoi(rArg.identity);
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->containsFollowing(stmtNum)) {
                        resultStore.createColumn(lArg, reader->getFollowing(stmtNum));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->containsFollowingT(stmtNum)) {
                        resultStore.createColumn(lArg, reader->getFollowingT(stmtNum));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->containsChild(stmtNum)) {
                        resultStore.createColumn(lArg, reader->getParent(stmtNum));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->containsChildT(stmtNum)) {
                        resultStore.createColumn(lArg, reader->getParentT(stmtNum));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::READ) {
                std::vector<int> read = reader->getAllReadStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : read) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : read) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : read) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : read) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::PRINT) {
                std::vector<int> prints = reader->getAllPrintStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : prints) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : prints) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : prints) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : prints) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::ASSIGN) {
                std::vector<int> assigns = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : assigns) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : assigns) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::CALL) {
                std::vector<int> calls = reader->getAllCallStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : calls) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : calls) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : calls) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : calls) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WHILE) {
                std::vector<int> whiles = reader->getAllWhileStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : whiles) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : whiles) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::IF) {
                std::vector<int> ifs = reader->getAllIfStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : stmtNums) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : ifs) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : stmtNums) {
                            for (int child : ifs) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else {
                // TODO: throw illegal argument, not allowed entity type for statRef
                return false;
            }
        } else if (lArg.entityType == SimpleProgram::DesignEntity::READ) {
            std::vector<int> reads = reader->getAllReadStmtNum();
            std::vector<std::string> lResults = {};
            std::vector<std::string> rResults = {};

            if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                int stmtNum = stoi(rArg.identity);
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    for (auto readStmtNum : reads) {
                        if (reader->containsFollowsRelationship(readStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(readStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    for (auto readStmtNum : reads) {
                        if (reader->containsFollowsTRelationship(readStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(readStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    for (auto readStmtNum : reads) {
                        if (reader->containsParentRelationship(readStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(readStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    for (auto readStmtNum : reads) {
                        if (reader->containsParentTRelationship(readStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(readStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::STMT) {
                std::vector<int> stmtNums = reader->getAllStatementNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : reads) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : reads) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : reads) {
                            for (int child : stmtNums) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : reads) {
                            for (int child : stmtNums) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::PRINT) {
                std::vector<int> prints = reader->getAllPrintStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : reads) {
                            for (int follower : prints) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : reads) {
                            for (int follower : prints) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : reads) {
                            for (int child : prints) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : reads) {
                            for (int child : prints) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::ASSIGN) {
                std::vector<int> assigns = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : reads) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : reads) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : reads) {
                            for (int child : assigns) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : reads) {
                            for (int child : assigns) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::CALL) {
                std::vector<int> calls = reader->getAllCallStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : reads) {
                            for (int follower : calls) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : reads) {
                            for (int follower : calls) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : reads) {
                            for (int child : calls) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : reads) {
                            for (int child : calls) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WHILE) {
                std::vector<int> whiles = reader->getAllWhileStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : reads) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : reads) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : reads) {
                            for (int child : whiles) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : reads) {
                            for (int child : whiles) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::IF) {
                std::vector<int> ifs = reader->getAllIfStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : reads) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : reads) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : reads) {
                            for (int child : ifs) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : reads) {
                            for (int child : ifs) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else {
                // TODO: throw illegal argument, not allowed entity type for statRef
                return false;
            }
        } else if (lArg.entityType == SimpleProgram::DesignEntity::PRINT) {
            std::vector<int> prints = reader->getAllPrintStmtNum();
            std::vector<std::string> lResults = {};
            std::vector<std::string> rResults = {};

            if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                int stmtNum = stoi(rArg.identity);
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    for (auto printStmtNum : prints) {
                        if (reader->containsFollowsRelationship(printStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(printStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    for (auto printStmtNum : prints) {
                        if (reader->containsFollowsTRelationship(printStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(printStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    for (auto printStmtNum : prints) {
                        if (reader->containsParentRelationship(printStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(printStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    for (auto printStmtNum : prints) {
                        if (reader->containsParentTRelationship(printStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(printStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::STMT) {
                std::vector<int> stmtNums = reader->getAllStatementNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : prints) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : prints) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : prints) {
                            for (int child : stmtNums) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : prints) {
                            for (int child : stmtNums) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::READ) {
                std::vector<int> reads = reader->getAllReadStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : prints) {
                            for (int follower : reads) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : prints) {
                            for (int follower : reads) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : prints) {
                            for (int child : reads) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : prints) {
                            for (int child : reads) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::ASSIGN) {
                std::vector<int> assigns = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : prints) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : prints) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : prints) {
                            for (int child : assigns) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : prints) {
                            for (int child : assigns) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::CALL) {
                std::vector<int> calls = reader->getAllCallStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : prints) {
                            for (int follower : calls) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : prints) {
                            for (int follower : calls) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : prints) {
                            for (int child : calls) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : prints) {
                            for (int child : calls) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WHILE) {
                std::vector<int> whiles = reader->getAllWhileStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : prints) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : prints) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : prints) {
                            for (int child : whiles) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : prints) {
                            for (int child : whiles) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::IF) {
                std::vector<int> ifs = reader->getAllIfStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : prints) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : prints) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : prints) {
                            for (int child : ifs) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : prints) {
                            for (int child : ifs) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else {
                // TODO: throw illegal argument, not allowed entity type for statRef
                return false;
            }
        } else if (lArg.entityType == SimpleProgram::DesignEntity::ASSIGN) {
            std::vector<int> assigns = reader->getAllAssignStmtNum();
            std::vector<std::string> lResults = {};
            std::vector<std::string> rResults = {};

            if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                int stmtNum = stoi(rArg.identity);
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    for (auto assignStmtNum : assigns) {
                        if (reader->containsFollowsRelationship(assignStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(assignStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    for (auto assignStmtNum : assigns) {
                        if (reader->containsFollowsTRelationship(assignStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(assignStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    for (auto assignStmtNum : assigns) {
                        if (reader->containsParentRelationship(assignStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(assignStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    for (auto printStmtNum : assigns) {
                        if (reader->containsParentTRelationship(printStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(printStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::STMT) {
                std::vector<int> stmtNums = reader->getAllStatementNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : assigns) {
                            for (int child : stmtNums) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : assigns) {
                            for (int child : stmtNums) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::READ) {
                std::vector<int> reads = reader->getAllReadStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : reads) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : reads) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : assigns) {
                            for (int child : reads) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : assigns) {
                            for (int child : reads) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::PRINT) {
                std::vector<int> prints = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : prints) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : prints) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : assigns) {
                            for (int child : prints) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : assigns) {
                            for (int child : prints) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::CALL) {
                std::vector<int> calls = reader->getAllCallStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : calls) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : calls) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : assigns) {
                            for (int child : calls) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : assigns) {
                            for (int child : calls) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WHILE) {
                std::vector<int> whiles = reader->getAllWhileStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : assigns) {
                            for (int child : whiles) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : assigns) {
                            for (int child : whiles) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::IF) {
                std::vector<int> ifs = reader->getAllIfStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : assigns) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : assigns) {
                            for (int child : ifs) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : assigns) {
                            for (int child : ifs) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else {
                // TODO: throw illegal argument, not allowed entity type for statRef
                return false;
            }
        } else if (lArg.entityType == SimpleProgram::DesignEntity::CALL) {
            std::vector<int> calls = reader->getAllCallStmtNum();
            std::vector<std::string> lResults = {};
            std::vector<std::string> rResults = {};

            if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                int stmtNum = stoi(rArg.identity);
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    for (auto callStmtNum : calls) {
                        if (reader->containsFollowsRelationship(callStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(callStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    for (auto callStmtNum : calls) {
                        if (reader->containsFollowsTRelationship(callStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(callStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    for (auto callStmtNum : calls) {
                        if (reader->containsParentRelationship(callStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(callStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    for (auto callStmtNum : calls) {
                        if (reader->containsParentTRelationship(callStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(callStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::STMT) {
                std::vector<int> stmtNums = reader->getAllStatementNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : calls) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : calls) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : calls) {
                            for (int child : stmtNums) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : calls) {
                            for (int child : stmtNums) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::READ) {
                std::vector<int> reads = reader->getAllReadStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : calls) {
                            for (int follower : reads) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : calls) {
                            for (int follower : reads) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : calls) {
                            for (int child : reads) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : calls) {
                            for (int child : reads) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::PRINT) {
                std::vector<int> prints = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : calls) {
                            for (int follower : prints) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : calls) {
                            for (int follower : prints) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : calls) {
                            for (int child : prints) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : calls) {
                            for (int child : prints) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::ASSIGN) {
                std::vector<int> assigns = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : calls) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : calls) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : calls) {
                            for (int child : assigns) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : calls) {
                            for (int child : assigns) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WHILE) {
                std::vector<int> whiles = reader->getAllWhileStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : calls) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : calls) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : calls) {
                            for (int child : whiles) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : calls) {
                            for (int child : whiles) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::IF) {
                std::vector<int> ifs = reader->getAllIfStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : calls) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : calls) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : calls) {
                            for (int child : ifs) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : calls) {
                            for (int child : ifs) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else {
                // TODO: throw illegal argument, not allowed entity type for statRef
                return false;
            }
        } else if (lArg.entityType == SimpleProgram::DesignEntity::WHILE) {
            std::vector<int> whiles = reader->getAllWhileStmtNum();
            std::vector<std::string> lResults = {};
            std::vector<std::string> rResults = {};

            if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                int stmtNum = stoi(rArg.identity);
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    for (auto whileStmtNum : whiles) {
                        if (reader->containsFollowsRelationship(whileStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(whileStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    for (auto whileStmtNum : whiles) {
                        if (reader->containsFollowsTRelationship(whileStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(whileStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    for (auto whileStmtNum : whiles) {
                        if (reader->containsParentRelationship(whileStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(whileStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    for (auto whileStmtNum : whiles) {
                        if (reader->containsParentTRelationship(whileStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(whileStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::STMT) {
                std::vector<int> stmtNums = reader->getAllStatementNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : whiles) {
                            for (int child : stmtNums) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : whiles) {
                            for (int child : stmtNums) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::READ) {
                std::vector<int> reads = reader->getAllReadStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : reads) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : reads) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : whiles) {
                            for (int child : reads) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : whiles) {
                            for (int child : reads) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::PRINT) {
                std::vector<int> prints = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : prints) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : prints) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : whiles) {
                            for (int child : prints) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : whiles) {
                            for (int child : prints) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::ASSIGN) {
                std::vector<int> assigns = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : whiles) {
                            for (int child : assigns) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : whiles) {
                            for (int child : assigns) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::CALL) {
                std::vector<int> calls = reader->getAllCallStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : calls) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : calls) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : whiles) {
                            for (int child : calls) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : whiles) {
                            for (int child : calls) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::IF) {
                std::vector<int> ifs = reader->getAllIfStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : whiles) {
                            for (int follower : ifs) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : whiles) {
                            for (int child : ifs) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : whiles) {
                            for (int child : ifs) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else {
                // TODO: throw illegal argument, not allowed entity type for statRef
                return false;
            }
        } else if (lArg.entityType == SimpleProgram::DesignEntity::WHILE) {
            std::vector<int> ifs = reader->getAllIfStmtNum();
            std::vector<std::string> lResults = {};
            std::vector<std::string> rResults = {};

            if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                int stmtNum = stoi(rArg.identity);
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    for (auto ifStmtNum : ifs) {
                        if (reader->containsFollowsRelationship(ifStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(ifStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    for (auto ifStmtNum : ifs) {
                        if (reader->containsFollowsTRelationship(ifStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(ifStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    for (auto ifStmtNum : ifs) {
                        if (reader->containsParentRelationship(ifStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(ifStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    for (auto ifStmtNum : ifs) {
                        if (reader->containsParentTRelationship(ifStmtNum, stmtNum)) {
                            lResults.push_back(std::to_string(ifStmtNum));
                        }
                    }
                    if (lResults.empty()) {
                        return false;
                    }

                    resultStore.createColumn(lArg, lResults);
                    return true;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        // TODO: WAIT FOR GETALLKEYS
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::STMT) {
                std::vector<int> stmtNums = reader->getAllStatementNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : stmtNums) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : ifs) {
                            for (int child : stmtNums) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : ifs) {
                            for (int child : stmtNums) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::READ) {
                std::vector<int> reads = reader->getAllReadStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : reads) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : reads) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : ifs) {
                            for (int child : reads) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : ifs) {
                            for (int child : reads) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::PRINT) {
                std::vector<int> prints = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : prints) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : prints) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : ifs) {
                            for (int child : prints) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : ifs) {
                            for (int child : prints) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::ASSIGN) {
                std::vector<int> assigns = reader->getAllAssignStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : assigns) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : ifs) {
                            for (int child : assigns) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : ifs) {
                            for (int child : assigns) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::CALL) {
                std::vector<int> calls = reader->getAllCallStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : calls) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : calls) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : ifs) {
                            for (int child : calls) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : ifs) {
                            for (int child : calls) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else if (rArg.entityType == SimpleProgram::DesignEntity::WHILE) {
                std::vector<int> whiles = reader->getAllIfStmtNum();

                if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWS) {
                    if (reader->hasFollowsRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::FOLLOWST) {
                    if (reader->hasFollowsTRelationship()) {
                        for (int followee : ifs) {
                            for (int follower : whiles) {
                                if (reader->containsFollowsTRelationship(followee, follower)) {
                                    lResults.push_back(std::to_string(followee));
                                    rResults.push_back(std::to_string(follower));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENT) {
                    if (reader->hasParentRelationship()) {
                        for (int parent : ifs) {
                            for (int child : whiles) {
                                if (reader->containsParentRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PARENTT) {
                    if (reader->hasParentTRelationship()) {
                        for (int parent : ifs) {
                            for (int child : whiles) {
                                if (reader->containsParentTRelationship(parent, child)) {
                                    lResults.push_back(std::to_string(parent));
                                    rResults.push_back(std::to_string(child));
                                }
                            }
                        }

                        if (lResults.empty()) {
                            return false;
                        }

                        std::unordered_map<std::string, std::vector<std::string>> table {
                                {lArg.identity, lResults},
                                {rArg.identity, rResults}
                        };
                        std::unordered_set<std::string> cols = {lArg.identity, rArg.identity};
                        resultStore.insertResult(std::make_shared<Result>(table, cols));
                        return true;
                    }
                    return false;
                } else {
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    return false;
                }
            } else {
                // TODO: throw illegal argument, not allowed entity type for statRef
                return false;
            }
        }

        return false;
    }
}