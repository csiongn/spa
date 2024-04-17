#include <string>
#include <unordered_map>

#include "WithClauseParser.h"
#include "QPS/Utils/ParseUtils.h"
#include "QPS/QuerySyntaxError.h"

WithClauseParser::WithClauseParser(std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>> &relationshipClauseTokens,
								   std::shared_ptr<ClauseValidator> &validator,
								   std::vector<PQL::Synonym> &declarations)
	: relationshipClauseTokens(relationshipClauseTokens), validator(validator), declarations(declarations) {}

std::vector<std::shared_ptr<QueryToken>> WithClauseParser::getWithClause() {

  int endPos = 4;
  if (relationshipClauseTokens->at(1)->getValue() == "not") {
	endPos = 5;
  }

  auto withClauseTokens = QueryEvaluator::ParseUtils::splitTokens(*relationshipClauseTokens, 0, endPos);
  relationshipClauseTokens =
	  std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(QueryEvaluator::ParseUtils::splitTokens(*relationshipClauseTokens,
																										 endPos,
																										 relationshipClauseTokens->size()));

  return withClauseTokens;
}

bool WithClauseParser::isNotClause(std::vector<std::shared_ptr<QueryToken>> &withClauseTokens) {
  return withClauseTokens[1]->getValue() == "not";
}

PQL::Clause WithClauseParser::parse(
	std::vector<std::shared_ptr<QueryToken>> &withClauseTokens) {
  std::vector<PQL::Synonym> args;

  int withClausePos = isNotClause(withClauseTokens) ? 2 : 1;

  auto withOperandTokens =
	  QueryEvaluator::ParseUtils::splitTokens(withClauseTokens, withClausePos, withClauseTokens.size());

  validator->validateWith(withOperandTokens);

  auto lOperandToken = withOperandTokens[0];
  auto rOperandToken = withOperandTokens[2];

  PQL::Synonym lOperandSynonym = QueryEvaluator::ParseUtils::createAttrSynonym(lOperandToken, declarations);
  PQL::Synonym rOperandSynonym = QueryEvaluator::ParseUtils::createAttrSynonym(rOperandToken, declarations);

  args.push_back(lOperandSynonym);
  args.push_back(rOperandSynonym);

  return {SimpleProgram::DesignAbstraction::WITH, args};
}
