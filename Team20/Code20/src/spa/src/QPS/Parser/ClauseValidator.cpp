#include <unordered_set>
#include "ClauseValidator.h"
#include "QPS/QuerySyntaxError.h"
#include "QPS/Utils/ParseUtils.h"

ClauseValidator::ClauseValidator(std::vector<PQL::Synonym>& declarations) : declarations(declarations), hasSemanticError(false) {}

bool ClauseValidator::isSynonym(std::shared_ptr<QueryToken> token) {
    return token->getType() == QPS::TokenType::NAME;
}

bool ClauseValidator::isValidIdent(std::shared_ptr<QueryToken> token) {
    return token->getType() == QPS::TokenType::CONSTANT_STRING && QueryEvaluator::ParseUtils::isName(token->getValue());
}

bool ClauseValidator::isStmtRef(std::shared_ptr<QueryToken> token) {
    std::unordered_set<SimpleProgram::DesignEntity> stmtTypes({
        SimpleProgram::DesignEntity::STMT,
        SimpleProgram::DesignEntity::ASSIGN, 
        SimpleProgram::DesignEntity::CALL, 
        SimpleProgram::DesignEntity::IF, 
        SimpleProgram::DesignEntity::PRINT, 
        SimpleProgram::DesignEntity::READ, 
        SimpleProgram::DesignEntity::WHILE});

    auto tokenType = token->getType();
    return tokenType == QPS::TokenType::WILDCARD 
        || tokenType == QPS::TokenType::INTEGER 
        || isSynonym(token) 
        && stmtTypes.find(QueryEvaluator::ParseUtils::getEntityType(token, declarations)) != stmtTypes.end();
}

bool ClauseValidator::isEntRef(std::shared_ptr<QueryToken> token) {
    std::unordered_set<SimpleProgram::DesignEntity> entTypes({
        SimpleProgram::DesignEntity::PROCEDURE,
        SimpleProgram::DesignEntity::VARIABLE, 
        SimpleProgram::DesignEntity::CONSTANT});

    auto tokenType = token->getType();
    return tokenType == QPS::TokenType::WILDCARD
        || isValidIdent(token)
        || isSynonym(token)
        && entTypes.find(QueryEvaluator::ParseUtils::getEntityType(token, declarations)) != entTypes.end();
}

bool ClauseValidator::isFactor(std::shared_ptr<QueryToken> token) {
    auto tokenType = token->getType();
    return tokenType == QPS::TokenType::INTEGER || tokenType == QPS::TokenType::NAME || isExpression(token);
}

bool ClauseValidator::isTerm(std::shared_ptr<QueryToken> token) {
    return isFactor(token);
}


bool ClauseValidator::isExpression(std::shared_ptr<QueryToken> token) {
    return token->getType() == QPS::TokenType::EXPRESSION || isTerm(token);
}

// bool ClauseValidator::isExpressionSpec(std::shared_ptr<QueryToken> token) {
//     return true;
// }

void ClauseValidator::setSemanticError() {
    hasSemanticError = true;
}

bool ClauseValidator::hasError() {
    return hasSemanticError;
}

void ClauseValidator::validateDeclarationExists(std::shared_ptr<QueryToken> synonymToken) {
    std::string id = synonymToken->getValue();
    for (auto& declaration : declarations) {
        if (declaration.identity == id) {
            return;
        }
    }
    setSemanticError();
}

void ClauseValidator::validateDeclarationExists(std::string synonymIdentity) {
    for (auto& declaration : declarations) {
        if (declaration.identity == synonymIdentity) {
            return;
        }
    }
    setSemanticError();
}

void ClauseValidator::validateHasSelectSynonym(std::vector<std::shared_ptr<QueryToken>>& clauseTokens) {
    if (clauseTokens.empty() || clauseTokens.size() == 1) {
        throw QuerySyntaxError("Syntax Error: No select synonym");
    };
}

void ClauseValidator::validateSelectClauseToken(std::shared_ptr<QueryToken> selectToken) {
    if (selectToken->getValue() != "Select") {
        throw QuerySyntaxError("Syntax Error: Select clause should come first");
    }
}

void ClauseValidator::validatePatternSynonym(std::shared_ptr<QueryToken>& patternToken) {
    std::unordered_set<SimpleProgram::DesignEntity> allowedTypes({
        SimpleProgram::DesignEntity::ASSIGN, 
        SimpleProgram::DesignEntity::IF, 
        SimpleProgram::DesignEntity::WHILE});

    auto patternEntityType = QueryEvaluator::ParseUtils::getEntityType(patternToken, declarations);

    if (allowedTypes.find(patternEntityType) == allowedTypes.end()) {
        throw QuerySyntaxError("Syntax Error: Invalid pattern synonym used");
    }
}

void ClauseValidator::validatePatternAssign(std::shared_ptr<QueryToken>& patternToken, int count) {
    if (count == 1) {
        if (!isStmtRef(patternToken)) {
            throw QuerySyntaxError("Syntax Error: First argument of pattern assign should be a statement reference");
        }
    } else if (count == 2) {
        if (!isExpressionSpec(patternToken)) {
            throw QuerySyntaxError("Syntax Error: Second argument of pattern assign should be an expression spec");
        }
    } else {
        throw QuerySyntaxError("Syntax Error: Pattern assign should only have two arguments");
    }
}