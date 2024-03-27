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
  auto declarationsValidator = std::make_shared<DeclarationsValidator>(DeclarationsValidator{});
  DeclarationsParser declarationsParser = DeclarationsParser(declarationTokens, semicolonPos, declarationsValidator);
  auto declarations = declarationsParser.parseDeclarations();

  auto clauseTokens = QueryEvaluator::ParseUtils::splitTokens(tokens, semicolonPos.back() + 1, tokens.size());
  auto clauseValidator = std::make_shared<ClauseValidator>(ClauseValidator(declarations));
  ClauseParser clauseParser = ClauseParser(clauseTokens, declarations, clauseValidator);
  auto selectSynonym = clauseParser.parseSelectClause();
  auto clauses = clauseParser.parseRelationshipClause();

  if (declarationsValidator->hasError() || clauseValidator->hasError()) {
	throw QuerySemanticError("Semantic Error");
  }

  PQL::Query query = PQL::Query(declarations, clauses, selectSynonym);
  return query;
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
  return semicolonPos;
}
