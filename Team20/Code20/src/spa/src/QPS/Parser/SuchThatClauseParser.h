#pragma once

#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"

class SuchThatClauseParser {
protected:
    std::vector<std::shared_ptr<QueryToken>> relationshipClauseTokens;
    std::vector<PQL::Synonym> declarations;

    public:
        explicit SuchThatClauseParser(std::vector<std::shared_ptr<QueryToken>>& relationshipClauseTokens, std::vector<PQL::Synonym>& declarations);
        std::vector<std::shared_ptr<QueryToken>> getSuchThatClause(const int start);
}