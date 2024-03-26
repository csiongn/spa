#pragma once

#include <vector>

#include "QPS/QueryToken.h"
#include "Models/PQL.h"

class DeclarationsValidator {
public:
    static void validateSynonymType(std::shared_ptr<QueryToken>& token);
    static void validateNoDuplicates(std::vector<PQL::Synonym>& declarations, std::shared_ptr<QueryToken>& token);
};