#pragma once

#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"
#include "ClauseValidator.h"

class PatternClauseParser {
    protected:
        std::vector<std::shared_ptr<QueryToken>> relationshipClauseTokens;
        ClauseValidator validator;

    public:
        explicit PatternClauseParser(std::vector<std::shared_ptr<QueryToken>>& relationshipClauseTokens, ClauseValidator& validator);
        std::vector<std::shared_ptr<QueryToken>> getPatternClause(const int start);
        SimpleProgram::DesignAbstraction getPatternClauseType(std::shared_ptr<QueryToken>& patternClauseToken);
        PQL::Clause parse(std::vector<std::shared_ptr<QueryToken>>& patternClauseTokens);
};