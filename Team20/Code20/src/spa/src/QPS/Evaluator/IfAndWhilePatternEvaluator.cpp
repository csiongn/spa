#include "IfAndWhilePatternEvaluator.h"

namespace QueryEvaluator {
    bool IfAndWhilePatternEvaluator::evaluate() {
        PQL::Synonym lArg = clause.arguments[1];

        if (lArg.entityType == SimpleProgram::DesignEntity::VARIABLE) {
            // (VAR, _)
            return getForwardRelationship();
        } else if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD){
            // (_, _)
            return hasAtLeastOneRelationship();
        } else {
            // ("IDENT", _)
            return hasRelationship();
        }
    }

    bool IfAndWhilePatternEvaluator::hasRelationship() {
        // handles (IDENT, _) & (IDENT, _, _)
        PQL::Synonym patternSyn = clause.arguments[0];
        PQL::Synonym lArg = clause.arguments[1];

        std::vector<int> stmtNums;
        if (patternSyn.entityType == SimpleProgram::DesignEntity::IF) {
            stmtNums = reader->getIfPatternStmtNum(lArg.identity);
        } else {
            stmtNums = reader->getWhilePatternStmtNum(lArg.identity);
        }

        if (stmtNums.empty()) {
            return false;
        }

        // add the if/while SYN to result store
        resultStore->createColumn(patternSyn, stmtNums);
        return true;
    }

    bool IfAndWhilePatternEvaluator::hasAtLeastOneRelationship() {
        // handles (_, _)
        // not adding IF_SYN/WHILE_SYN to result store, result same as all if/while statements
        // already added during initialise SYN in QueryEvaluator
        PQL::Synonym patternSyn = clause.arguments[0];
        if (patternSyn.entityType == SimpleProgram::DesignEntity::IF) {
            return reader->hasIfPattern();
        }

        return reader->hasWhilePattern();
    }

    bool IfAndWhilePatternEvaluator::getForwardRelationship() {
        // Handles (VAR, _)
        return getSynonymWildcard();
    }

    bool IfAndWhilePatternEvaluator::getSynonymWildcard() {
        // Handles (VAR, _) / (VAR, _, _)
        PQL::Synonym patternSyn = clause.arguments[0];
        PQL::Synonym lArg = clause.arguments[1];
        std::vector<std::string> vars;
        std::vector<int> stmtNums;

        if (patternSyn.entityType == SimpleProgram::DesignEntity::IF) {
            vars = reader->getIfPatternVariable();
        } else {
            vars = reader->getWhilePatternVariable();
        }

        if (vars.empty()) {
            return false;
        }

        std::vector<std::vector<std::string>> table = {{}, {}};
        std::vector<std::string> colNames = {patternSyn.identity, lArg.identity};
        std::unordered_map<std::string, size_t> colNameToIndex;
        for (size_t i = 0; i < colNames.size(); i++) {
            colNameToIndex[colNames[i]] = i;
        }
        Result newResult{table, colNames, colNameToIndex};

        for (auto const &var: vars) {
            if (patternSyn.entityType == SimpleProgram::DesignEntity::IF) {
                stmtNums = reader->getIfPatternStmtNum(var);
            } else {
                stmtNums = reader->getWhilePatternStmtNum(var);
            }
            for (int stmtNum: stmtNums) {
                newResult.addRow({std::to_string(stmtNum), var});
            }
        }

        resultStore->insertResult(std::make_shared<Result>(newResult));
        return true;
    }

    bool IfAndWhilePatternEvaluator::getLeftResults() {
        // same as getSynonymWildCard in this case
        return false;
    }

    bool IfAndWhilePatternEvaluator::isAlwaysFalse() {
        // No always false case for if/while pattern
        return false;
    }

    bool IfAndWhilePatternEvaluator::getReversedRelationship() {
        // No RIGHT SYN for if/while pattern
        return false;
    }

    bool IfAndWhilePatternEvaluator::getRightResults() {
        // No RIGHT SYN for if/while pattern
        return false;
    }

    bool IfAndWhilePatternEvaluator::getWildcardSynonym() {
        // No RIGHT SYN for if/while pattern
        return false;
    }

    bool IfAndWhilePatternEvaluator::getDoubleSynonym() {
        // No RIGHT SYN for if/while pattern
        return false;
    }
}