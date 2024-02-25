#include <unordered_set>

#include "AssignPatternEvaluator.h"
#include "../Utils/ParseUtils.h"

namespace QueryEvaluator {

    bool AssignPatternEvaluator::evaluate() {
        PQL::Synonym lArg = clause.arguments[1];
        PQL::Synonym rArg = clause.arguments[2];

        if (lArg.entityType == rArg.entityType && lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
            // handles (_, _)
            return hasAtLeastOneRelationship();
        }

        if (lArg.entityType == SimpleProgram::DesignEntity::VARIABLE) {
            // Handles (VAR, EXPR), (VAR, PARTIAL_EXPR), (VAR, _)
            return getForwardRelationship();
        }

        if (lArg.entityType == SimpleProgram::DesignEntity::IDENT) {
            if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                // (IDENT, _)
                return hasAtLeastOneRelationship();
            }

            // Handles (IDENT, EXPR), (IDENT, PARTIAL_EXPR)
            return hasRelationship();
        }

        if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
            // handles (_, EXPR), (_, PARTIAL_EXPR)
            return hasAtLeastOneRelationship();
        }

        return false;
    }

    bool AssignPatternEvaluator::isAlwaysFalse() {
        // No always false case for assign pattern
        return false;
    }

    bool AssignPatternEvaluator::hasRelationship() {
        // handles (IDENT, EXPR) & (IDENT, PARTIAL_EXPR)
        PQL::Synonym assignSyn = clause.arguments[0];
        PQL::Synonym lArg = clause.arguments[1];
        PQL::Synonym rArg = clause.arguments[2];

        if (rArg.entityType == SimpleProgram::DesignEntity::EXPR) {
            // (IDENT, EXPR)
            return hasRelationship(lArg.identity, rArg.identity);
        } else {
            // (IDENT, PARTIAL_EXPR)
            std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(rArg.identity);
            std::shared_ptr<ExprNode> exprNode = std::make_shared<VariableNode>(std::get<0>(tup));

            // get all partial expr nodes
            std::vector<std::shared_ptr<ExprNode>> partialExprNodes = getAllPartialNodes(exprNode);

            // use the partial expr nodes to get all VAR
            std::vector<std::string> vars = getAllAssignVar(partialExprNodes);

            if (vars.empty()) {
                return false;
            }

            // use the vars and partial expr nodes  to get the related assign stmt nums
            // get ASSIGN_SYN without duplicates
            std::vector<int> assignSynResults = getAssignSynResults(vars, partialExprNodes);
            resultStore->createColumn(assignSyn, assignSynResults);
            return true;
        }
    }

    bool AssignPatternEvaluator::hasRelationship(const std::string &ident, const std::string &expr) {
        std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(expr);
        std::shared_ptr<ExprNode> exprNode = std::make_shared<VariableNode>(std::get<0>(tup));
        std::vector<int> assignStmtNums = reader->getAssignPatternStmtNum(ident, exprNode->getHashValue());

        if (assignStmtNums.empty()) {
            return false;
        }

        // add ASSIGN_SYN to result store
        resultStore->createColumn(clause.arguments[0], assignStmtNums);
        return true;
    }

    bool AssignPatternEvaluator::hasAtLeastOneRelationship() {
        // handles (_, _) and (IDENT, _), (_, EXPR), (_, PARTIAL_EXPR)
        PQL::Synonym assignSyn = clause.arguments[0];
        PQL::Synonym lArg = clause.arguments[1];
        PQL::Synonym rArg = clause.arguments[2];

        if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
            std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(rArg.identity);
            std::shared_ptr<ExprNode> exprNode = std::make_shared<VariableNode>(std::get<0>(tup));
            if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
                // not adding ASSIGN_SYN to result store, result same as all assign statements
                // already added during initialise SYN in QueryEvaluator
                return !reader->getAllAssignStmtNum().empty();
            } else if (rArg.entityType == SimpleProgram::DesignEntity::EXPR) {
                // (_, EXPR)
                std::vector<int> assignStmtNums = reader->getAssignPatternRHSStmtNum(exprNode->getHashValue());
                if (assignStmtNums.empty()) {
                    return false;
                }

                resultStore->createColumn(assignSyn, assignStmtNums);
                return true;
            } else {
                // (_, PARTIAL_EXPR)
                std::vector<std::shared_ptr<ExprNode>> partialExprNodes = getAllPartialNodes(exprNode);

                std::unordered_set<int> assignStmtNumsSet;
                for (auto const &partialExprNode: partialExprNodes) {
                    std::vector<int> stmtNums = reader->getAssignPatternRHSStmtNum(partialExprNode->getHashValue());
                    assignStmtNumsSet.insert(stmtNums.begin(), stmtNums.end());
                }

                if (assignStmtNumsSet.empty()) {
                    return false;
                }

                std::vector<int> assignSynResults{assignStmtNumsSet.begin(), assignStmtNumsSet.end()};
                resultStore->createColumn(assignSyn, assignSynResults);
                return true;
            }
        } else {
            // (IDENT, _)
            std::vector<int> assignStmtNums = reader->getAssignPatternLHSStmtNum(lArg.identity);

            if (assignStmtNums.empty()) {
                return false;
            }

            // add ASSIGN_SYN to result store
            resultStore->createColumn(assignSyn, assignStmtNums);
            return true;
        }
    }

    bool AssignPatternEvaluator::getForwardRelationship() {
        // Handles (VAR, EXPR), (VAR, PARTIAL_EXPR), (VAR, _)
        PQL::Synonym assignSyn = clause.arguments[0];
        PQL::Synonym lArg = clause.arguments[1];
        PQL::Synonym rArg = clause.arguments[2];

        std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(rArg.identity);
        std::shared_ptr<ExprNode> exprNode = std::make_shared<VariableNode>(std::get<0>(tup));
        if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
            // (VAR, _)
            return getSynonymWildcard();
        } else {
            // (VAR, EXPR), (VAR, PARTIAL_EXPR)
            return getLeftResults();
        }
    }

    bool AssignPatternEvaluator::getReversedRelationship() {
        // No RIGHT SYN for assign pattern
        return false;
    }

    bool AssignPatternEvaluator::getLeftResults() {
        // (VAR, EXPR), (VAR, PARTIAL_EXPR)
        PQL::Synonym assignSyn = clause.arguments[0];
        PQL::Synonym lArg = clause.arguments[1];
        PQL::Synonym rArg = clause.arguments[2];

        std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(rArg.identity);
        std::shared_ptr<ExprNode> exprNode = std::make_shared<VariableNode>(std::get<0>(tup));

        if (std::get<1>(tup) && std::get<2>(tup)) {
            // (VAR, PARTIAL_EXPR)
            std::vector<std::shared_ptr<ExprNode>> partialExprNodes = getAllPartialNodes(exprNode);

            // use the partialExprNodes to get all VAR
            std::vector<std::string> vars = getAllAssignVar(partialExprNodes);

            if (vars.empty()) {
                return false;
            }

            // add VAR_SYN to result store
            resultStore->createColumn(lArg, vars);

            // add ASSIGN_SYN to result store
            std::vector<int> assignSynResults = getAssignSynResults(vars, partialExprNodes);
            resultStore->createColumn(assignSyn, assignSynResults);
            return true;
        } else {
            // (VAR, EXPR)
            std::vector<std::string> lResults = reader->getAssignPatternLHS(exprNode->getHashValue());
            if (lResults.empty()) {
                return false;
            }

            // add VAR_SYN to result store
            resultStore->createColumn(lArg, lResults);

            // add ASSIGN_SYN to result store
            std::vector<int> assignSynResults = getAssignSynResults(lResults, {exprNode});
            resultStore->createColumn(assignSyn, assignSynResults);
            return true;
        }
    }

    bool AssignPatternEvaluator::getSynonymWildcard() {
        // Handles (VAR, _)
        PQL::Synonym assignSyn = clause.arguments[0];
        PQL::Synonym lArg = clause.arguments[1];

        std::vector<std::string> lResults = reader->getAssignPatternLHS();
        if (lResults.empty()) {
            return false;
        }

        // add all variable syn to result store
        resultStore->createColumn(lArg, lResults);

        // add ASSIGN_SYN to result store
        std::unordered_set<int> assignStmtNumsSet;
        for (auto const &var: lResults) {
            std::vector<int> stmtNums = reader->getAssignPatternLHSStmtNum(var);
            assignStmtNumsSet.insert(stmtNums.begin(), stmtNums.end());
        }

        std::vector<int> assignSynResults{assignStmtNumsSet.begin(), assignStmtNumsSet.end()};
        resultStore->createColumn(assignSyn, assignSynResults);
        return true;
        return false;
    }

    bool AssignPatternEvaluator::getRightResults() {
        // No RIGHT SYN for assign pattern
        return false;
    }

    bool AssignPatternEvaluator::getWildcardSynonym() {
        // No RIGHT SYN for assign pattern
        return false;
    }

    bool AssignPatternEvaluator::getDoubleSynonym() {
        // No RIGHT SYN for assign pattern
        return false;
    }

    std::vector<std::shared_ptr<ExprNode>> AssignPatternEvaluator::getAllPartialNodes(const std::shared_ptr<ExprNode> &exprNode) {
        // get all RHS
        std::vector<size_t> allHashes = reader->getAssignPatternRHS();

        // use all RHS to get all nodes
        std::vector<size_t> filteredHashes;
        std::vector<std::shared_ptr<ExprNode>> allNodes;
        for (auto const &hash: allHashes) {
            std::vector<std::shared_ptr<ExprNode>> nodes = reader->getAssignPatternRHSExprNodePtr(hash);
            allNodes.reserve(allNodes.size() + nodes.size());
            allNodes.insert(allNodes.end(), nodes.begin(), nodes.end());
        }

        // filter nodes to only those that contains the partial expr
        std::vector<std::shared_ptr<ExprNode>> filteredNodes;
        filteredNodes.reserve(allNodes.size());
        for (auto const &node: allNodes) {
            if (node->containsSubtree(*(exprNode))) {
                filteredNodes.push_back(node);
            }
        }

        return filteredNodes;
    }

    std::vector<std::string> AssignPatternEvaluator::getAllAssignVar(const std::vector<std::shared_ptr<ExprNode>> &nodes) {
        std::vector<std::string> vars;
        for (auto const &node: nodes) {
            std::vector<std::string> leftVars = reader->getAssignPatternLHS(node->getHashValue());
            vars.reserve(vars.size() + leftVars.size());
            vars.insert(vars.end(), leftVars.begin(), leftVars.end());
        }
        return vars;
    }

    std::vector<int> AssignPatternEvaluator::getAssignSynResults(const std::vector<std::string>& vars, const std::vector<std::shared_ptr<ExprNode>>& exprNodes) {
        std::unordered_set<int> assignSynResultsSet;
        for (auto const &var: vars) {
            for (auto const &exprNode: exprNodes) {
                std::vector<int> assignStmtNums = reader->getAssignPatternStmtNum(var, exprNode->getHashValue());
                assignSynResultsSet.reserve(assignSynResultsSet.size() + assignStmtNums.size());
                assignSynResultsSet.insert(assignStmtNums.begin(), assignStmtNums.end());
            }
        }

        // add ASSIGN_SYN to result store
        std::vector<int> assignSynResults;
        assignSynResults.reserve(assignSynResultsSet.size());
        assignSynResults.insert(assignSynResults.end(), assignSynResultsSet.begin(), assignSynResultsSet.end());

        return assignSynResults;
    }
}