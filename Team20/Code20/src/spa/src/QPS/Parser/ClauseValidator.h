#pragma once

#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"
class ClauseValidator {
    protected:
        bool hasSemanticError;
        std::vector<PQL::Synonym> declarations;

    private:
        bool isValidIdent(std::shared_ptr<QueryToken> token);

        bool isStmtRef(std::shared_ptr<QueryToken> token);

        bool isEntRef(std::shared_ptr<QueryToken> token);

        bool isElement(std::shared_ptr<QueryToken> token);

        bool isExpression(std::shared_ptr<QueryToken> token);

        bool isExpressionSpec(std::vector<std::shared_ptr<QueryToken>>& tokens);
    
    public:
        explicit ClauseValidator(std::vector<PQL::Synonym>& declarations);

        void setSemanticError();

        bool hasError();

        bool isSynonym(std::shared_ptr<QueryToken> token);

        bool isPartialExpression(std::shared_ptr<QueryToken> token);

        void validateStmtRef(std::shared_ptr<QueryToken> token);

        void validateEntRef(std::shared_ptr<QueryToken> token);

        void validateDeclarationExists(std::shared_ptr<QueryToken> synonymToken);

        void validateDeclarationExists(std::string synonymIdentity);

        void validateSelectClauseToken(std::shared_ptr<QueryToken> selectToken);

        static void validateHasSelectSynonym(std::vector<std::shared_ptr<QueryToken>> &clauseTokens);

        void validateSuchThatClause();

        void validatePatternSynonym(std::shared_ptr<QueryToken>& patternToken);

        void validatePatternAssign(std::vector<std::shared_ptr<QueryToken>>& patternArgs);

        void validatePatternIf(std::vector<std::shared_ptr<QueryToken>>& patternArgs);

        void validatePatternWhile(std::vector<std::shared_ptr<QueryToken>>& patternArgs);

        void validatePatternArgs(SimpleProgram::DesignAbstraction patternType, std::vector<std::shared_ptr<QueryToken>>& patternArgs);
};