#pragma once

#include <vector>
#include "Models/PQL.h"
#include "QPS/QueryToken.h"
#include "DeclarationsValidator.h"

class DeclarationsParser {
protected:
    std::vector<std::shared_ptr<QueryToken>> declarationTokens;
    std::vector<std::shared_ptr<QueryToken>> initialDeclarations;
    std::vector<std::shared_ptr<QueryToken>> usedDeclarations;
    std::vector<int> semicolonPos;
    DeclarationsValidator validator;

public:
    explicit DeclarationsParser(std::vector<std::shared_ptr<QueryToken>> declarationTokens, std::vector<int> semicolonPos);
    std::vector<PQL::Synonym> parseDeclarations();
};