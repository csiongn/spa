#include "ClauseParser.h"
#include "ClauseValidator.h"
#include "QPS/QuerySyntaxError.h"
#include "QPS/QueryToken.h"
#include "QPS/Utils/ParseUtils.h"
#include "PatternClauseParser.h"
#include "SuchThatClauseParser.h"

ClauseParser::ClauseParser(std::vector<std::shared_ptr<QueryToken>>& clauseTokens, std::vector<PQL::Synonym>& declarations)
    : clauseTokens(clauseTokens), declarations(declarations), validator(ClauseValidator(declarations)) {}

PQL::Synonym ClauseParser::getDeclarationUsed(std::shared_ptr<QueryToken> synonymToken) {
    std::string id = synonymToken->getValue();
    for (auto &declaration : declarations) {
        if (declaration.identity == id) {
            return declaration;
        }
    }
}

PQL::Synonym ClauseParser::getDeclarationUsed(std::string synonymIdentity) {
    for (auto &declaration : declarations) {
        if (declaration.identity == synonymIdentity) {
            return declaration;
        }
    }
}

std::vector<PQL::Synonym> ClauseParser::parseSelectClause() {
    ClauseValidator::validateHasSelectSynonym(clauseTokens);
    std::vector<PQL::Synonym> selectSynonyms;
    auto selectToken = clauseTokens[0];
    validator.validateSelectClauseToken(selectToken);
    auto nextToken = clauseTokens[1];
    
    if (nextToken->getType() == QPS::TokenType::TUPLE) {
        auto tupleSynonyms = QueryEvaluator::ParseUtils::splitTuple(nextToken);
        for (auto& tupleSynonym : tupleSynonyms) {
            validator.validateDeclarationExists(tupleSynonym);
            auto declarationUsed = getDeclarationUsed(tupleSynonym);
            SimpleProgram::DesignEntity entityType = declarationUsed.entityType;
            auto synonym = QueryEvaluator::ParseUtils::createSynonym(entityType, tupleSynonym);
            selectSynonyms.push_back(synonym);
        }
    } else {
        validator.validateDeclarationExists(nextToken);
        auto declarationUsed = getDeclarationUsed(nextToken);
        SimpleProgram::DesignEntity entityType = declarationUsed.entityType;
        auto synonym = QueryEvaluator::ParseUtils::createSynonym(entityType, nextToken);
        selectSynonyms.push_back(synonym);
    }

    return selectSynonyms;
}

std::vector<PQL::Clause> ClauseParser::parseRelationshipClause() {
    std::vector<PQL::Clause> clauses;

    std::vector<std::shared_ptr<QueryToken>> relationshipClauseTokens = QueryEvaluator::ParseUtils::splitTokens(clauseTokens, 2, clauseTokens.size());
    PatternClauseParser patternClauseParser = PatternClauseParser(relationshipClauseTokens, declarations);
    SuchThatClauseParser suchThatClauseParser = SuchThatClauseParser(relationshipClauseTokens, declarations);

    SimpleProgram::DesignAbstraction prevRelationship;

    while (!relationshipClauseTokens.empty()) {
        auto currToken = relationshipClauseTokens[0];
        std::string tokenValue = currToken->getValue();

        if (tokenValue == "pattern") {
            auto patternClauseTokens = patternClauseParser.getPatternClause(0);
            PQL::Clause patternClause = patternClauseParser.parse(patternClauseTokens);
            auto patternType = patternClause.clauseType;
            prevRelationship = patternType;
        } else if (tokenValue == "such") {
            auto suchThatClauseTokens = suchThatClauseParser.getSuchThatClause(0);
            PQL::Clause suchThatClause = suchThatClauseParser.parse(suchThatClauseTokens);
            auto suchThatType = suchThatClause.clauseType;
            prevRelationship = suchThatType;
        } else if (tokenValue == "with") {
            
        } else if (tokenValue == "and") {

        } else {
            throw QuerySyntaxError("Syntax Error: Invalid relationship clause");
        }

    }

    return clauses;
}