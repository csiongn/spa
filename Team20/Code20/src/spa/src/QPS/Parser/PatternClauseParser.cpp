#include "PatternClauseParser.h"
#include "QPS/Utils/ParseUtils.h"

PatternClauseParser::PatternClauseParser(std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>>& relationshipClauseTokens,
    std::shared_ptr<ClauseValidator>& validator, std::vector<PQL::Synonym>& declarations) 
    : relationshipClauseTokens(relationshipClauseTokens), validator(validator), declarations(declarations) {}

std::vector<std::shared_ptr<QueryToken>> PatternClauseParser::getPatternClause(const int start) {
    std::vector<std::shared_ptr<QueryToken>> patternClauseTokens;
    int curr = start;
    while (curr < relationshipClauseTokens->size()) {
        auto token = relationshipClauseTokens->at(curr);

        auto tokenValue = token->getValue();
        if (tokenValue == ")") {
            curr++;
            break;
        }
        curr++;
    }

    patternClauseTokens = QueryEvaluator::ParseUtils::splitTokens(*relationshipClauseTokens, start, curr);
    relationshipClauseTokens = std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(QueryEvaluator::ParseUtils::splitTokens(*relationshipClauseTokens, curr, relationshipClauseTokens->size()));
    return patternClauseTokens;
}

SimpleProgram::DesignAbstraction PatternClauseParser::getPatternClauseType(std::shared_ptr<QueryToken>& patternClauseToken) {
    auto entity = QueryEvaluator::ParseUtils::getEntityType(patternClauseToken, declarations);

    if (entity == SimpleProgram::DesignEntity::ASSIGN) {
        return SimpleProgram::DesignAbstraction::PATTERN_ASSIGN;
    } else if (entity == SimpleProgram::DesignEntity::WHILE) {
        return SimpleProgram::DesignAbstraction::PATTERN_WHILE;
    } else if (entity == SimpleProgram::DesignEntity::IF) {
        return SimpleProgram::DesignAbstraction::PATTERN_IF;
    } else {
        validator->setSemanticError();
        return SimpleProgram::DesignAbstraction::INVALID;
    }
}

PQL::Clause PatternClauseParser::parse(std::vector<std::shared_ptr<QueryToken>>& patternClauseTokens) {
    std::vector<PQL::Synonym> patternArgs;
    auto patternSynonymToken = patternClauseTokens[1];

    validator->validateDeclarationExists(patternSynonymToken);
    validator->validatePatternSynonym(patternSynonymToken);

    auto patternClauseType = getPatternClauseType(patternSynonymToken);
    std::vector<std::shared_ptr<QueryToken>> cleanedPatternTokens = QueryEvaluator::ParseUtils::removeBracketsAndCommas(patternClauseTokens);
    std::vector<std::shared_ptr<QueryToken>> patternArgsTokens = QueryEvaluator::ParseUtils::splitTokens(cleanedPatternTokens, 2, cleanedPatternTokens.size());

    auto patternSynonymEntityType = QueryEvaluator::ParseUtils::getEntityType(patternSynonymToken, declarations);
    auto patternSynonym = QueryEvaluator::ParseUtils::createSynonym(patternSynonymEntityType, patternSynonymToken);

    patternArgs.push_back(patternSynonym);

    validator->validatePatternArgs(patternClauseType, patternArgsTokens);
    if (patternClauseType == SimpleProgram::DesignAbstraction::PATTERN_ASSIGN) {
        auto largToken = patternArgsTokens[0];
        std::shared_ptr<QueryToken> rargToken;

        bool isPartialExpression = false;
        std::string partialExpressionId;

        if (patternArgsTokens.size() == 4) {
            isPartialExpression = true;
            std::vector<std::shared_ptr<QueryToken>> rargTokens = QueryEvaluator::ParseUtils::splitTokens(patternArgsTokens, 1, patternArgsTokens.size());

            for (const auto& token : rargTokens) {
                partialExpressionId += token->getValue();
            }
        } else {
            rargToken = patternArgsTokens[1];
        }
        SimpleProgram::DesignEntity largTokenType; 
        if (validator->isSynonym(largToken)) {
            largTokenType = QueryEvaluator::ParseUtils::getEntityType(largToken, declarations);
        } else {
            largTokenType = largToken->getType() == QPS::TokenType::WILDCARD
                ? SimpleProgram::DesignEntity::WILDCARD 
                : SimpleProgram::DesignEntity::IDENT;
        }
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(largTokenType, largToken));

        if (isPartialExpression) {
            patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::PARTIAL_EXPR, partialExpressionId));
        } else if (rargToken->getType() == QPS::TokenType::WILDCARD) {
            patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::WILDCARD, rargToken));
        } else {
            patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::EXPR, rargToken));
        }

    } else if (patternClauseType == SimpleProgram::DesignAbstraction::PATTERN_WHILE) {
        auto largToken = patternArgsTokens[0];
        auto rargToken = patternArgsTokens[1];
        SimpleProgram::DesignEntity largTokenType;
        if (validator->isSynonym(largToken)) {
            largTokenType = QueryEvaluator::ParseUtils::getEntityType(largToken, declarations);
        } else {
            largTokenType = largToken->getType() == QPS::TokenType::WILDCARD
                                ? SimpleProgram::DesignEntity::WILDCARD
                                : SimpleProgram::DesignEntity::IDENT;
        }
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(largTokenType, largToken));

        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::WILDCARD, rargToken));
    } else if (patternClauseType == SimpleProgram::DesignAbstraction::PATTERN_IF) {
        auto largToken = patternArgsTokens[0];
        auto margToken = patternArgsTokens[1];
        auto rargToken = patternArgsTokens[2];
        SimpleProgram::DesignEntity largTokenType;
        if (validator->isSynonym(largToken)) {
            largTokenType = QueryEvaluator::ParseUtils::getEntityType(largToken, declarations);
        } else {
            largTokenType = largToken->getType() == QPS::TokenType::WILDCARD
                                ? SimpleProgram::DesignEntity::WILDCARD
                                : SimpleProgram::DesignEntity::IDENT;
        }
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(largTokenType, largToken));
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::WILDCARD, margToken));
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::WILDCARD, rargToken));
    } else {
    }
    
    return {patternClauseType, patternArgs};
}
