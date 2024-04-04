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

PQL::Clause WithClauseParser::parse(
	std::vector<std::shared_ptr<QueryToken>> &withClauseTokens) {
  std::vector<PQL::Synonym> args;

  auto withOperandTokens = QueryEvaluator::ParseUtils::splitTokens(withClauseTokens, 1, withClauseTokens.size());

  validator->validateWith(withOperandTokens);

  auto lOperandToken = withOperandTokens[0];
  auto rOperandToken = withOperandTokens[2];

  PQL::Synonym lOperandSynonym = createSynonymFromAttrToken(lOperandToken);

  auto rOperandTokenType = rOperandToken->getType();
  PQL::Synonym rOperandSynonym = PQL::Synonym::createInvalidSynonym();

  if (rOperandTokenType == QPS::TokenType::ATTRIBUTE_NAME || rOperandTokenType == QPS::TokenType::ATTRIBUTE_CONSTANT
	  || rOperandTokenType == QPS::TokenType::ATTRIBUTE_VALUE) {
	rOperandSynonym = createSynonymFromAttrToken(rOperandToken);
  } else {
	SimpleProgram::DesignEntity rOperandEntity = SimpleProgram::DesignEntity::INVALID;
	SimpleProgram::AttributeRef rOperandAttrRef = SimpleProgram::AttributeRef::NO_REF;

	if (rOperandTokenType == QPS::TokenType::INTEGER) {
	  rOperandEntity = SimpleProgram::DesignEntity::INTEGER;
	} else if (rOperandTokenType == QPS::TokenType::CONSTANT_STRING) {
	  rOperandEntity = SimpleProgram::DesignEntity::IDENT;
	} else {
	  throw QuerySyntaxError("Syntax Error: Invalid with clause");
	}

	rOperandSynonym = QueryEvaluator::ParseUtils::createSynonym(rOperandEntity, rOperandToken->getValue());
  }

  args.push_back(lOperandSynonym);
  args.push_back(rOperandSynonym);

  return {SimpleProgram::DesignAbstraction::WITH, args};
}
