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

QueryParser::QueryParser(std::vector<std::shared_ptr<QueryToken>> tokens) : tokens(std::move(tokens)), pos(0) {}

//PQL::Query QueryParser::parse() {
//    std::vector<int> semicolonPos = parseSemicolons();
//    auto declarationTokens = splitTokens(0, semicolonPos.back() + 1);
//    DeclarationsParser declarationsParser = DeclarationsParser(declarationTokens, semicolonPos);
//    auto declarations = declarationsParser.parseDeclarations();
//
//
//    auto clauseTokens = QueryEvaluator::ParseUtils::splitTokens(semicolonPos.back() + 1, tokens.size());
//    ClauseParser clauseParser = ClauseParser(clauseTokens, declarations);
//
//  PQL::Synonym selectSynonym = parseSelectClause();
//  clauses = parseClauses();
//  // checkErrors();
//
//  PQL::Query query = PQL::Query(usedDeclarations, clauses, selectSynonym);
//    return query;
//}

PQL::Query QueryParser::parse() {
    return PQL::Query(std::vector{PQL::Synonym(SimpleProgram::DesignEntity{}, "1")}, std::vector{PQL::Clause(SimpleProgram::DesignAbstraction{}, std::vector{PQL::Synonym(SimpleProgram::DesignEntity{}, "2")})}, PQL::Synonym(SimpleProgram::DesignEntity{}, "3"));
}

std::vector<int> QueryParser::parseSemicolons() {
    std::vector<int> semicolonPos;
    int lastPos = -1;
    for (int i = 0; i < tokens.size(); i++) {
        auto curr = tokens[i];
        if (curr->getValue() == ";") {
            semicolonPos.push_back(i);
        }
    }
    pos = lastPos;
    return semicolonPos;
}
