#include "PatternClauseParser.h"
#include "QPS/Utils/ParseUtils.h"

PatternClauseParser::PatternClauseParser(std::vector<std::shared_ptr<QueryToken>>& relationshipClauseTokens, ClauseValidator& validator) 
    : relationshipClauseTokens(relationshipClauseTokens), validator(validator) {}

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
    auto entity = validator.getDeclarationUsed(patternClauseToken);

    if (entity.entityType == SimpleProgram::DesignEntity::ASSIGN) {
        return SimpleProgram::DesignAbstraction::PATTERN_ASSIGN;
    } else if (entity.entityType == SimpleProgram::DesignEntity::WHILE) {
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
    std::vector<std::shared_ptr<QueryToken>> argTokens = QueryEvaluator::ParseUtils::removeBracketsAndCommas(patternClauseTokens);

    auto patternSynonymEntityType = validator.getDeclarationUsed(patternSynonymToken).entityType;
    auto patternSynonym = QueryEvaluator::ParseUtils::createSynonym(patternSynonymEntityType, patternSynonymToken);

    patternArgs.push_back(patternSynonym);

    int count = 1;

    for (const auto& argToken : argTokens) {
        if (patternClauseType == SimpleProgram::DesignAbstraction::PATTERN_ASSIGN) {
            validator.validatePatternAssign(argToken, count);
        } else if (patternClauseType == SimpleProgram::DesignAbstraction::PATTERN_WHILE) {
            
        } else {
            
        }
        count++;
    }
    

    return;
}
