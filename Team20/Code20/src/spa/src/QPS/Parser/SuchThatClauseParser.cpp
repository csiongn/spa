#include "SuchThatClauseParser.h"
#include "QPS/Utils/ParseUtils.h"

SuchThatClauseParser::SuchThatClauseParser(std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>>& relationshipClauseTokens,
    std::shared_ptr<ClauseValidator>& validator, std::vector<PQL::Synonym>& declarations) : validator(validator), relationshipClauseTokens(relationshipClauseTokens), 
        declarations(declarations) {}

std::vector<std::shared_ptr<QueryToken>> SuchThatClauseParser::getSuchThatClause(const int start) {
    std::vector<std::shared_ptr<QueryToken>> suchThatClauseTokens;
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

    suchThatClauseTokens = QueryEvaluator::ParseUtils::splitTokens(*relationshipClauseTokens, start, curr);
    relationshipClauseTokens = std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(QueryEvaluator::ParseUtils::splitTokens(*relationshipClauseTokens, curr, relationshipClauseTokens->size()));
    return suchThatClauseTokens;
}

PQL::Clause SuchThatClauseParser::parse(std::vector<std::shared_ptr<QueryToken>> &suchThatClauseTokens) {
    return PQL::Clause(SimpleProgram::DesignAbstraction{}, std::vector{PQL::Synonym(SimpleProgram::DesignEntity{}, "2")});
}