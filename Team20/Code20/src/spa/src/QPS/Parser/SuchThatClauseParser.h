#pragma once

#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"
#include "ClauseValidator.h"

class SuchThatClauseParser {
protected:
    std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>>& relationshipClauseTokens;
    std::shared_ptr<ClauseValidator>& validator;
    std::vector<PQL::Synonym> declarations;

    public:
        explicit SuchThatClauseParser(std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>>& relationshipClauseTokens, std::shared_ptr<ClauseValidator>& validator, std::vector<PQL::Synonym>& declarations);
        std::vector<std::shared_ptr<QueryToken>> getSuchThatClause(const int start);
        PQL::Clause parse(std::vector<std::shared_ptr<QueryToken>>& suchThatClauseTokens);
};