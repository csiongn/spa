#pragma once

#include <vector>

#include "QPS/QueryToken.h"
#include "Models/PQL.h"

class DeclarationsValidator {
    protected:
        bool hasSemanticError;

    public:
        explicit DeclarationsValidator();

        void setSemanticError();

        bool hasError();

        static void validateSynonymType(std::shared_ptr<QueryToken>& token);

        void validateNoDuplicates(std::vector<PQL::Synonym>& declarations, std::shared_ptr<QueryToken>& token);

        static void validateSynonymName(std::shared_ptr<QueryToken>& token);
};