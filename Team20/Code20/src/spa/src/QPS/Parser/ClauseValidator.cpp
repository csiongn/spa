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

bool ClauseValidator::isExpression(std::shared_ptr<QueryToken> token) {
    return token->getType() == QPS::TokenType::EXPRESSION || token->getType() == QPS::TokenType::CONSTANT_STRING;
}

bool ClauseValidator::isExpressionSpec(std::shared_ptr<QueryToken> token) {
    if (!isExpression(token)) {
        return false;
    }
    std::string value = token->getValue();
    bool hasOnlyOneUnderscore = (value.find('_') == 0 || value.find('_') == value.size() - 1) 
        && (value.find('_') == value.rfind('_')) 
        && (value.find('_') != std::string::npos);
    if (hasOnlyOneUnderscore) {
        return false;
    }

    bool hasTwoUnderscores = (value.find('_') == 0 && value.rfind('_') == value.size() - 1);
    return hasTwoUnderscores || value.find('_') == std::string::npos;
}

bool ClauseValidator::isPartialExpression(std::shared_ptr<QueryToken> token) {
    if (!isExpression(token)) {
        return false;
    }
    std::string value = token->getValue();
    bool hasOnlyOneUnderscore = (value.find('_') == 0 || value.find('_') == value.size() - 1) 
        && (value.find('_') == value.rfind('_')) 
        && (value.find('_') != std::string::npos);
    return hasOnlyOneUnderscore;
}

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

void ClauseValidator::validatePatternAssign(std::vector<std::shared_ptr<QueryToken>> &patternTokens) {
    for (int count = 0; count < patternTokens.size(); count++) {
        auto currToken = patternTokens[count];
        if (count == 0) {
            if (!isStmtRef(currToken)) {
                throw QuerySyntaxError("Syntax Error: First argument of pattern assign should be a statement reference");
            }
            if (isSynonym(currToken)) {
                validateDeclarationExists(currToken);
            }
        }
        else if (count == 1) {
            if (!isExpressionSpec(currToken)) {
                throw QuerySyntaxError("Syntax Error: Second argument of pattern assign should be an expression spec");
            }
        }
        else {
            throw QuerySyntaxError("Syntax Error: Pattern assign should have two arguments");
        }
    }
}

void ClauseValidator::validatePatternIf(std::vector<std::shared_ptr<QueryToken>>& patternTokens) {
    for (int count = 0; count < patternTokens.size(); count++) {
        auto currToken = patternTokens[count];
        if (count == 0) {
            if (!isEntRef(currToken)) {
                throw QuerySyntaxError("Syntax Error: First argument of pattern assign should be a statement reference");
            }
            if (isSynonym(currToken)) {
                validateDeclarationExists(currToken);
            }
        } else if (count == 1 || count == 2) {
            if (currToken->getType() == QPS::TokenType::WILDCARD) {
                throw QuerySyntaxError("Syntax Error: Second argument and third argument of pattern if should be a wildcard");
            }
        } else {
            throw QuerySyntaxError("Syntax Error: Pattern if should have three arguments");
        }
    }
}

void ClauseValidator::validatePatternWhile(std::vector<std::shared_ptr<QueryToken>> &patternTokens) {
    for (int count = 0; count < patternTokens.size(); count++) {
        auto currToken = patternTokens[count];
        if (count == 0) {
            if (!isEntRef(currToken)) {
                throw QuerySyntaxError("Syntax Error: First argument of pattern assign should be a statement reference");
            }
            if (isSynonym(currToken)) {
                validateDeclarationExists(currToken);
            }
        } else if (count == 1) {
            if (currToken->getType() == QPS::TokenType::WILDCARD) {
                throw QuerySyntaxError("Syntax Error: Second argument of pattern while should be a wildcard");
            }
        } else {
            throw QuerySyntaxError("Syntax Error: Pattern while should have two arguments");
        }
    }
}

void ClauseValidator::validatePatternArgs(SimpleProgram::DesignAbstraction patternType, std::vector<std::shared_ptr<QueryToken>>& patternArgs) {
    if (patternType == SimpleProgram::DesignAbstraction::PATTERN_ASSIGN) {
        validatePatternAssign(patternArgs);
    } else if (patternType == SimpleProgram::DesignAbstraction::PATTERN_WHILE) {
        validatePatternWhile(patternArgs);
    } else {
        validatePatternIf(patternArgs);
    }
}