#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

#include "QueryParser.h"
#include "QPS/QuerySemanticError.h"
#include "QPS/QuerySyntaxError.h"
#include "QPS/Utils/ParseUtils.h"
#include "DeclarationsParser.h"
#include "ClauseParser.h"

QueryParser::QueryParser(std::vector<std::shared_ptr<QueryToken>> tokens) : tokens(std::move(tokens)) {}

PQL::Query QueryParser::parse() {
    std::vector<int> semicolonPos = parseSemicolons();
    auto declarationTokens = QueryEvaluator::ParseUtils::splitTokens(tokens, 0, semicolonPos.back() + 1);
    DeclarationsParser declarationsParser = DeclarationsParser(declarationTokens, semicolonPos);
    auto declarations = declarationsParser.parseDeclarations();


    auto clauseTokens = QueryEvaluator::ParseUtils::splitTokens(tokens, semicolonPos.back() + 1, tokens.size());
    ClauseParser clauseParser = ClauseParser(clauseTokens, declarations);
    auto selectSynonym = clauseParser.parseSelectClause();
    auto clauses = clauseParser.parseRelationshipClause();

    PQL::Query query = PQL::Query(declarations, clauses, selectSynonym);
    return query;
}

// PQL::Query QueryParser::parse() {
//     return PQL::Query(std::vector{PQL::Synonym(SimpleProgram::DesignEntity{}, "1")}, std::vector{PQL::Clause(SimpleProgram::DesignAbstraction{}, std::vector{PQL::Synonym(SimpleProgram::DesignEntity{}, "2")})}, PQL::Synonym(SimpleProgram::DesignEntity{}, "3"));
// }

std::vector<int> QueryParser::parseSemicolons() {
    std::vector<int> semicolonPos;
    int lastPos = -1;
    for (int i = 0; i < tokens.size(); i++) {
        auto curr = tokens[i];
        if (curr->getValue() == ";") {
            semicolonPos.push_back(i);
        }
    }
    return semicolonPos;
}
