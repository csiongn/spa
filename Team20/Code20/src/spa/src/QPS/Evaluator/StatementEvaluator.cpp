#include "StatementEvaluator.h"
#include "ClauseEvaluator.h"


namespace QueryEvaluator {

    bool StatementEvaluator::evaluate() {
        // Handles clauses with only stmtRef: FOLLOWS/FOLLOWS*/PARENT/PARENT*

        if (isAlwaysFalse()) {
            // Handles 7 cases: two same stmtNum (2,2) (overlaps with STMT_NO_1, STMT_NO_2), 7 same synonym (Y,Y)
            return false;
        }

        PQL::Synonym lArg = clause.arguments[0];
        PQL::Synonym rArg = clause.arguments[1];

        // Handles 2 cases: (STMT_NO_1, STMT_NO_2), (WILDCARD, WILDCARD)
        if (lArg.entityType == rArg.entityType) {
            if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
                return hasRelationship();
            } else if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                return hasAtLeastOneRelationship();
            }
        }

        // Handles 8 cases: 7 * (STMT_NUM, SYN), (STMT_NUM, WILDCARD)
        if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
            return getForwardRelationship();
        }

        // Handles 8 cases: 7 * (SYN, STMT_NUM), (WILDCARD, STMT_NUM)
        if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
            return getReversedRelationship();
        }

        // Handles 7 cases: 7 * (WILDCARD, SYN)
        if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
            return getSynonymWildcard();
        }

        // Handles 7 cases: 7 * (SYN, WILDCARD)
        if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
            return getWildcardSynonym();
        }

        // Handles 42 cases: 7 * 6 (SYN_A, SYN_B) WHERE SYN_A != SYN_B
        return getDoubleSynonym();
    }

    bool StatementEvaluator::isAlwaysFalse() {
        if (clause.arguments[0].entityType == SimpleProgram::DesignEntity::WILDCARD
            || clause.arguments[1].entityType == SimpleProgram::DesignEntity::WILDCARD) {
            return false;
        }

        return clause.arguments[0] == clause.arguments[1];
    }

    bool StatementEvaluator::hasRelationship() {
        // handles XXX(STMT_NO, STMT_NO)
        int leftStmtNum = stoi(clause.arguments[0].identity);
        int rightStmtNum = stoi(clause.arguments[1].identity);

        return hasRelationship(clause.clauseType, leftStmtNum, rightStmtNum);
    }

    bool StatementEvaluator::hasRelationship(const SimpleProgram::DesignAbstraction &relationship, int leftStmtNum,
                                             int rightStmtNum) {
        switch (clause.clauseType) {
            case SimpleProgram::DesignAbstraction::FOLLOWS:
                return reader->containsFollowsRelationship(leftStmtNum, rightStmtNum);
            case SimpleProgram::DesignAbstraction::FOLLOWST:
                return reader->containsFollowsTRelationship(leftStmtNum, rightStmtNum);
            case SimpleProgram::DesignAbstraction::PARENT:
                return reader->containsParentRelationship(leftStmtNum, rightStmtNum);
            case SimpleProgram::DesignAbstraction::PARENTT:
                return reader->containsParentTRelationship(leftStmtNum, rightStmtNum);
            default:
                // TODO: throw illegal argument, not allowed relationship type for statement only queries
                return false;
        }
    }

    bool StatementEvaluator::hasAtLeastOneRelationship() {
        // handles XXX(_, _)
        switch (clause.clauseType) {
            case SimpleProgram::DesignAbstraction::FOLLOWS:
                return reader->hasFollowsRelationship();
            case SimpleProgram::DesignAbstraction::FOLLOWST:
                return reader->hasFollowsTRelationship();
            case SimpleProgram::DesignAbstraction::PARENT:
                return reader->hasParentRelationship();
            case SimpleProgram::DesignAbstraction::PARENTT:
                return reader->hasParentTRelationship();
            default:
                // TODO: throw illegal argument, not allowed relationship type for statement only queries
                return false;
        }
    }

    bool StatementEvaluator::getForwardRelationship() {
        // handles XXX(STMT_NUM, SYNONYM) and (STMT_NUM, WILDCARD)
        PQL::Synonym rArg = clause.arguments[1];
        int leftStmtNum = stoi(clause.arguments[0].identity);

        if (rArg.entityType == SimpleProgram::DesignEntity::STMT
            || rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {

            std::vector<int> rResults;
            switch (clause.clauseType) {
                case SimpleProgram::DesignAbstraction::FOLLOWS:
                    rResults = reader->getFollows(leftStmtNum);
                    break;
                case SimpleProgram::DesignAbstraction::FOLLOWST:
                    rResults = reader->getFollowsT(leftStmtNum);
                    break;
                case SimpleProgram::DesignAbstraction::PARENT:
                    rResults = reader->getChild(leftStmtNum);
                    break;
                case SimpleProgram::DesignAbstraction::PARENTT:
                    rResults = reader->getChildT(leftStmtNum);
                    break;
                default:
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    rResults = {};
            }

            if (rArg.entityType == SimpleProgram::DesignEntity::STMT && !rResults.empty()) {
                resultStore.createColumn(rArg, rResults);
            }
            return !rResults.empty();
        }

        return getRightResults();
    }

    bool StatementEvaluator::getRightResults() {
        PQL::Synonym rArg = clause.arguments[1];
        int leftStmtNum = stoi(clause.arguments[0].identity);

        std::vector<std::string> rResults = {};
        std::vector<int> rightStmtNums = getStmtNums(rArg);

        for (auto rightStmtNum : rightStmtNums) {
            if (hasRelationship(clause.clauseType, leftStmtNum, rightStmtNum)) {
                rResults.push_back(std::to_string(rightStmtNum));
            }
        }

        if (rResults.empty()) {
            return false;
        }

        resultStore.createColumn(rArg, rResults);
        return true;
    }

    bool StatementEvaluator::getWildcardSynonym() {
        // handles (_, SYN)
        PQL::Synonym rArg = clause.arguments[1];
        std::vector<int> rResults = getUniqueValues(rArg);

        if (rResults.empty()) {
            return false;
        }

        resultStore.createColumn(rArg, rResults);
        return true;
    }

    bool StatementEvaluator::getReversedRelationship() {
        // handles XXX(SYNONYM, STMT_NUM) and (WILDCARD, STMT_NUM)
        PQL::Synonym lArg = clause.arguments[0];
        int rightStmtNum = stoi(clause.arguments[1].identity);

        if (lArg.entityType == SimpleProgram::DesignEntity::STMT
            || lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {

            std::vector<int> lResults;
            switch (clause.clauseType) {
                case SimpleProgram::DesignAbstraction::FOLLOWS:
                    lResults = reader->getFollowing(rightStmtNum);
                    break;
                case SimpleProgram::DesignAbstraction::FOLLOWST:
                    lResults = reader->getFollowingT(rightStmtNum);
                    break;
                case SimpleProgram::DesignAbstraction::PARENT:
                    lResults = reader->getParent(rightStmtNum);
                    break;
                case SimpleProgram::DesignAbstraction::PARENTT:
                    lResults = reader->getParentT(rightStmtNum);
                    break;
                default:
                    // TODO: throw illegal argument, not allowed relationship type for statement only queries
                    lResults = {};
            }

            if (lArg.entityType == SimpleProgram::DesignEntity::STMT && !lResults.empty()) {
                resultStore.createColumn(lArg, lResults);
            }
            return !lResults.empty();
        }

        return getLeftResults();
    }

    bool StatementEvaluator::getLeftResults() {
        PQL::Synonym lArg = clause.arguments[0];
        int rightStmtNum = stoi(clause.arguments[1].identity);

        std::vector<std::string> lResults = {};
        std::vector<int> leftStmtNums = getStmtNums(lArg);

        for (auto leftStmtNum : leftStmtNums) {
            if (hasRelationship(clause.clauseType, leftStmtNum, rightStmtNum)) {
                lResults.push_back(std::to_string(leftStmtNum));
            }
        }

        if (lResults.empty()) {
            return false;
        }

        resultStore.createColumn(lArg, lResults);
        return true;
    }

    bool StatementEvaluator::getSynonymWildcard() {
        // handles (SYN, _)
        PQL::Synonym lArg = clause.arguments[0];
        std::vector<int> lResults = getUniqueKeys(lArg);

        if (lResults.empty()) {
            return false;
        }

        resultStore.createColumn(lArg, lResults);
        return true;
    }

    std::vector<int> StatementEvaluator::getStmtNums(const PQL::Synonym &syn) {
        switch (syn.entityType) {
            case SimpleProgram::DesignEntity::STMT:
                return reader->getAllStatementNum();
            case SimpleProgram::DesignEntity::READ:
                return reader->getAllReadStmtNum();
            case SimpleProgram::DesignEntity::PRINT:
                return reader->getAllPrintStmtNum();
            case SimpleProgram::DesignEntity::ASSIGN:
                return reader->getAllAssignStmtNum();
            case SimpleProgram::DesignEntity::CALL:
                return reader->getAllCallStmtNum();
            case SimpleProgram::DesignEntity::WHILE:
                return reader->getAllWhileStmtNum();
            case SimpleProgram::DesignEntity::IF:
                return reader->getAllIfStmtNum();
            default:
                // TODO: throw illegal argument, not allowed entity type for statement reference
                return {};
        }
    }

    std::vector<int> StatementEvaluator::getUniqueKeys(const PQL::Synonym &syn) {
        std::vector<int> keyStmtNums;
        switch (clause.clauseType) {
            case SimpleProgram::DesignAbstraction::FOLLOWS:
            case SimpleProgram::DesignAbstraction::FOLLOWST:
                keyStmtNums = reader->getFolloweeStmts();
                break;
            case SimpleProgram::DesignAbstraction::PARENT:
            case SimpleProgram::DesignAbstraction::PARENTT:
                keyStmtNums = reader->getParentStmts();
                break;
            default:
                // TODO: throw illegal argument, not allowed relationship type for statement only queries
                return {};
        }

        if (syn.entityType == SimpleProgram::DesignEntity::STMT) {
            return keyStmtNums;
        }

        std::vector<int> synStmtNums = getStmtNums(syn);
        std::vector<int> intersection;

        return getIntersection(synStmtNums, keyStmtNums);
    }

    std::vector<int> StatementEvaluator::getUniqueValues(const PQL::Synonym &syn) {
        std::vector<int> valueStmtNums;
        switch (clause.clauseType) {
            case SimpleProgram::DesignAbstraction::FOLLOWS:
            case SimpleProgram::DesignAbstraction::FOLLOWST:
                valueStmtNums = reader->getFollowerStmts();
                break;
            case SimpleProgram::DesignAbstraction::PARENT:
            case SimpleProgram::DesignAbstraction::PARENTT:
                valueStmtNums = reader->getChildStmts();
                break;
            default:
                // TODO: throw illegal argument, not allowed relationship type for statement only queries
                valueStmtNums = {};
        }

        if (syn.entityType == SimpleProgram::DesignEntity::STMT) {
            return valueStmtNums;
        }

        std::vector<int> synStmtNums = getStmtNums(syn);
        std::vector<int> intersection;

        return getIntersection(synStmtNums, valueStmtNums);
    }

    std::vector<int> StatementEvaluator::getIntersection(std::vector<int> &v1, std::vector<int> &v2) {
        std::vector<int> intersection;
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        std::set_intersection(v1.begin(), v1.end(),
                              v2.begin(), v2.end(), std::back_inserter(intersection));
        return intersection;
    }

    bool StatementEvaluator::getDoubleSynonym() {
        PQL::Synonym lArg = clause.arguments[0];
        PQL::Synonym rArg = clause.arguments[1];
        std::vector<int> lValues = getStmtNums(lArg);
        std::vector<int> rValues = getStmtNums(rArg);
        std::vector<std::string> lResults = {};
        std::vector<std::string> rResults = {};

        for (auto v1 : lValues) {
            for (auto v2: rValues) {
                if (hasRelationship(clause.clauseType, v1, v2)) {
                    lResults.push_back(std::to_string(v1));
                    rResults.push_back(std::to_string(v2));
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
}