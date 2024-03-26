#include "PatternClauseParser.h"
#include "QPS/Utils/ParseUtils.h"

PatternClauseParser::PatternClauseParser(std::vector<std::shared_ptr<QueryToken>>& relationshipClauseTokens, 
    ClauseValidator& validator, std::vector<PQL::Synonym>& declarations) 
    : relationshipClauseTokens(relationshipClauseTokens), validator(validator), declarations(declarations) {}

std::vector<std::shared_ptr<QueryToken>> PatternClauseParser::getPatternClause(const int start) {
    std::vector<std::shared_ptr<QueryToken>> patternClauseTokens;
    int curr = start;
    while (curr < relationshipClauseTokens.size()) {
        auto token = relationshipClauseTokens[start];

        auto tokenValue = token->getValue();
        if (tokenValue == ")") {
            break;
        }
        curr++;
    }

    patternClauseTokens = QueryEvaluator::ParseUtils::splitTokens(relationshipClauseTokens, start, curr + 1);
    relationshipClauseTokens = QueryEvaluator::ParseUtils::splitTokens(relationshipClauseTokens, curr + 1, relationshipClauseTokens.size());
    return patternClauseTokens;
}

SimpleProgram::DesignAbstraction PatternClauseParser::getPatternClauseType(std::shared_ptr<QueryToken>& patternClauseToken) {
    auto entity = QueryEvaluator::ParseUtils::getEntityType(patternClauseToken, declarations);

    if (entity == SimpleProgram::DesignEntity::ASSIGN) {
        return SimpleProgram::DesignAbstraction::PATTERN_ASSIGN;
    } else if (entity == SimpleProgram::DesignEntity::WHILE) {
        return SimpleProgram::DesignAbstraction::PATTERN_WHILE;
    } else {
        return SimpleProgram::DesignAbstraction::PATTERN_IF;
    }
}

PQL::Clause PatternClauseParser::parse(std::vector<std::shared_ptr<QueryToken>>& patternClauseTokens) {
    std::vector<PQL::Synonym> patternArgs;
    auto patternSynonymToken = patternClauseTokens[0];

    validator.validateDeclarationExists(patternSynonymToken);
    validator.validatePatternSynonym(patternSynonymToken);

    auto patternClauseType = getPatternClauseType(patternSynonymToken);
    std::vector<std::shared_ptr<QueryToken>> cleanedPatternTokens = QueryEvaluator::ParseUtils::removeBracketsAndCommas(patternClauseTokens);
    std::vector<std::shared_ptr<QueryToken>> patternArgsTokens = QueryEvaluator::ParseUtils::splitTokens(cleanedPatternTokens, 1, cleanedPatternTokens.size());

    auto patternSynonymEntityType = QueryEvaluator::ParseUtils::getEntityType(patternSynonymToken, declarations);
    auto patternSynonym = QueryEvaluator::ParseUtils::createSynonym(patternSynonymEntityType, patternSynonymToken);

    patternArgs.push_back(patternSynonym);

    validator.validatePatternArgs(patternClauseType, patternArgsTokens);
    if (patternClauseType == SimpleProgram::DesignAbstraction::PATTERN_ASSIGN) {
        auto largToken = patternArgsTokens[0];
        auto rargToken = patternArgsTokens[1];
        SimpleProgram::DesignEntity largTokenType; 
        if (validator.isSynonym(largToken)) {
            largTokenType = QueryEvaluator::ParseUtils::getEntityType(largToken, declarations);
        } else {
            largTokenType = largToken->getType() == QPS::TokenType::WILDCARD
                ? SimpleProgram::DesignEntity::WILDCARD 
                : SimpleProgram::DesignEntity::IDENT;
        }
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(largTokenType, largToken));

        SimpleProgram::DesignEntity rargTokenType = validator.isPartialExpression(rargToken) 
            ? SimpleProgram::DesignEntity::PARTIAL_EXPR 
            : SimpleProgram::DesignEntity::EXPR;
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(rargTokenType, rargToken));

    } else if (patternClauseType == SimpleProgram::DesignAbstraction::PATTERN_WHILE) {
        auto largToken = patternArgsTokens[0];
        auto rargToken = patternArgsTokens[1];
        SimpleProgram::DesignEntity largTokenType;
        if (validator.isSynonym(largToken)) {
            largTokenType = QueryEvaluator::ParseUtils::getEntityType(largToken, declarations);
        } else {
            largTokenType = largToken->getType() == QPS::TokenType::WILDCARD
                                ? SimpleProgram::DesignEntity::WILDCARD
                                : SimpleProgram::DesignEntity::IDENT;
        }
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(largTokenType, largToken));

        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::WILDCARD, rargToken));
    } else {
        auto largToken = patternArgsTokens[0];
        auto margToken = patternArgsTokens[1];
        auto rargToken = patternArgsTokens[2];
        SimpleProgram::DesignEntity largTokenType;
        if (validator.isSynonym(largToken)) {
            largTokenType = QueryEvaluator::ParseUtils::getEntityType(largToken, declarations);
        } else {
            largTokenType = largToken->getType() == QPS::TokenType::WILDCARD
                                ? SimpleProgram::DesignEntity::WILDCARD
                                : SimpleProgram::DesignEntity::IDENT;
        }
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(largTokenType, largToken));
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::WILDCARD, margToken));
        patternArgs.push_back(QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity::WILDCARD, rargToken));
    }
    
    return PQL::Clause(patternClauseType, patternArgs);
}
