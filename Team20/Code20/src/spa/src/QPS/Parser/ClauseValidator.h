#pragma once

#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"
class ClauseValidator {
    protected:
        bool hasSemanticError;
        std::vector<PQL::Synonym> declarations;

    private:
        bool isSynonym(std::shared_ptr<QueryToken> token);

        bool isValidIdent(std::shared_ptr<QueryToken> token);

        bool isStmtRef(std::shared_ptr<QueryToken> token);

        bool isEntRef(std::shared_ptr<QueryToken> token);

        bool isElement(std::shared_ptr<QueryToken> token);

        bool isExpression(std::shared_ptr<QueryToken> token);

        bool isExpressionSpec(std::shared_ptr<QueryToken> token);

        bool isFactor(std::shared_ptr<QueryToken> token);

        bool isTerm(std::shared_ptr<QueryToken> token);
    
    public:
        explicit ClauseValidator(std::vector<PQL::Synonym>& declarations);

        void setSemanticError();

        bool hasError();

        PQL::Synonym getDeclarationUsed(std::shared_ptr<QueryToken> synonymToken);

        void validateDeclarationExists(std::shared_ptr<QueryToken> synonymToken);

        void validateDeclarationExists(std::string synonymIdentity);

        void validateSelectClauseToken(std::shared_ptr<QueryToken> selectToken);

        static void validateHasSelectSynonym(std::vector<std::shared_ptr<QueryToken>> &clauseTokens);

        void validateSuchThatClause();

        void validatePatternSynonym(std::shared_ptr<QueryToken>& patternToken);

        void validatePatternAssign(std::shared_ptr<QueryToken>& patternToken, int count);
};