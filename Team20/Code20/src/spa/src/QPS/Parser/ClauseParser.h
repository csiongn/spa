#pragma once

#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"
#include "ClauseValidator.h"

class ClauseParser {
protected:
    std::vector<std::shared_ptr<QueryToken>> clauseTokens;
    std::vector<PQL::Synonym>& declarations;
    ClauseValidator validator;

public:
    explicit ClauseParser(std::vector<std::shared_ptr<QueryToken>>& clauseTokens, std::vector<PQL::Synonym>& declarations);
    PQL::Synonym getDeclarationUsed(std::shared_ptr<QueryToken> synonymToken);
    PQL::Synonym getDeclarationUsed(std::string synonymIdentity);
    std::vector<PQL::Synonym> parseSelectClause();
    std::vector<PQL::Clause> parseRelationshipClause(); 
};
