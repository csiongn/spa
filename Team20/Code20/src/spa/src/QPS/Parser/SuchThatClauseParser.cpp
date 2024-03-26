#include "SuchThatClauseParser.h"
#include "QPS/Utils/ParseUtils.h"

SuchThatClauseParser::SuchThatClauseParser(std::vector<std::shared_ptr<QueryToken>>& relationshipClauseTokens, 
    ClauseValidator& validator, std::vector<PQL::Synonym>& declarations) : validator(validator), relationshipClauseTokens(relationshipClauseTokens), 
        declarations(declarations) {}

std::vector<std::shared_ptr<QueryToken>> SuchThatClauseParser::getSuchThatClause(const int start) {
    std::vector<std::shared_ptr<QueryToken>> suchThatClauseTokens;
    int curr = start;
    while (curr < relationshipClauseTokens.size()) {
        auto token = relationshipClauseTokens[start];

        auto tokenValue = token->getValue();
        if (tokenValue == ")") {
            break;
        }
        curr++;
    }

    suchThatClauseTokens = QueryEvaluator::ParseUtils::splitTokens(relationshipClauseTokens, start, curr + 1);
    relationshipClauseTokens = QueryEvaluator::ParseUtils::splitTokens(relationshipClauseTokens, curr + 1, relationshipClauseTokens.size());
    return suchThatClauseTokens;
}

PQL::Clause SuchThatClauseParser::parse(std::vector<std::shared_ptr<QueryToken>> &suchThatClauseTokens) {
    return PQL::Clause(SimpleProgram::DesignAbstraction{}, std::vector{PQL::Synonym(SimpleProgram::DesignEntity{}, "2")});
}