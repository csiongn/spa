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

  auto withClauseTokens = QueryEvaluator::ParseUtils::splitTokens(*relationshipClauseTokens, 0, 4);
  relationshipClauseTokens =
	  std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(QueryEvaluator::ParseUtils::splitTokens(*relationshipClauseTokens,
																										 4,
																										 relationshipClauseTokens->size()));

  return withClauseTokens;
}

PQL::Synonym WithClauseParser::createSynonymFromAttrToken(std::shared_ptr<QueryToken> attrToken) {
  std::vector<std::string> splitted = QueryEvaluator::ParseUtils::splitAttrToken(attrToken);
  auto synonymValue = splitted[0];
  auto attrRefStr = splitted[1];
  auto entityType = QueryEvaluator::ParseUtils::getEntityType(synonymValue, declarations);
  auto attrRef = getAttrRef(attrRefStr);
  return QueryEvaluator::ParseUtils::createSynonym(entityType, synonymValue, attrRef);
}

SimpleProgram::AttributeRef WithClauseParser::getAttrRef(std::string attrRefStr) {
  std::unordered_map<std::string, SimpleProgram::AttributeRef> strToAttrRef = {
	  {"procName", SimpleProgram::AttributeRef::NAME},
	  {"varName", SimpleProgram::AttributeRef::NAME},
	  {"value", SimpleProgram::AttributeRef::INTEGER},
	  {"stmt#", SimpleProgram::AttributeRef::INTEGER}
  };

  return strToAttrRef.at(attrRefStr);
}

PQL::Synonym WithClauseParser::createWithClauseSynonym(std::shared_ptr<QueryToken> operandToken) {
  PQL::Synonym operandSynonym = PQL::Synonym::createInvalidSynonym();
  QPS::TokenType operandTokenType = operandToken->getType();
  if (operandTokenType == QPS::TokenType::ATTRIBUTE_NAME || operandTokenType == QPS::TokenType::ATTRIBUTE_CONSTANT
	  || operandTokenType == QPS::TokenType::ATTRIBUTE_VALUE) {
	operandSynonym = createSynonymFromAttrToken(operandToken);
  } else {
	SimpleProgram::DesignEntity operandEntity = SimpleProgram::DesignEntity::INVALID;

	if (operandTokenType == QPS::TokenType::INTEGER) {
	  operandEntity = SimpleProgram::DesignEntity::INTEGER;
	} else if (operandTokenType == QPS::TokenType::CONSTANT_STRING) {
	  operandEntity = SimpleProgram::DesignEntity::IDENT;
	} else {
	  throw QuerySyntaxError("Syntax Error: Invalid with clause");
	}

	operandSynonym = QueryEvaluator::ParseUtils::createSynonym(operandEntity, operandToken->getValue());
  }
  return operandSynonym;
}

PQL::Clause WithClauseParser::parse(
	std::vector<std::shared_ptr<QueryToken>> &withClauseTokens) {
  std::vector<PQL::Synonym> args;

  auto withOperandTokens = QueryEvaluator::ParseUtils::splitTokens(withClauseTokens, 1, withClauseTokens.size());

  validator->validateWith(withOperandTokens);

  auto lOperandToken = withOperandTokens[0];
  auto rOperandToken = withOperandTokens[2];

  PQL::Synonym lOperandSynonym = createWithClauseSynonym(lOperandToken);
  PQL::Synonym rOperandSynonym = createWithClauseSynonym(rOperandToken);

  args.push_back(lOperandSynonym);
  args.push_back(rOperandSynonym);

  return {SimpleProgram::DesignAbstraction::WITH, args};
}
