#include "SuchThatClauseParser.h"

#include <memory>
#include <string>
#include <unordered_map>

#include "QPS/QuerySyntaxError.h"
#include "QPS/Utils/ParseUtils.h"

SuchThatClauseParser::SuchThatClauseParser(
	std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>>
	&relationshipClauseTokens,
	std::shared_ptr<ClauseValidator> &validator,
	std::vector<PQL::Synonym> &declarations)
	: validator(validator),
	  relationshipClauseTokens(relationshipClauseTokens),
	  declarations(declarations) {}

std::vector<std::shared_ptr<QueryToken>>
SuchThatClauseParser::getSuchThatClause() {
  return QueryEvaluator::ParseUtils::getClause(relationshipClauseTokens);
}

SimpleProgram::DesignAbstraction SuchThatClauseParser::getSuchThatClauseType(
	std::shared_ptr<QueryToken> &suchThatClauseToken) {
  std::unordered_map<std::string, SimpleProgram::DesignAbstraction>
	  designAbstractionMap = {
	  {"Modifies", SimpleProgram::DesignAbstraction::MODIFIESS},
	  {"Uses", SimpleProgram::DesignAbstraction::USESS},
	  {"Parent", SimpleProgram::DesignAbstraction::PARENT},
	  {"Parent*", SimpleProgram::DesignAbstraction::PARENTT},
	  {"Follows", SimpleProgram::DesignAbstraction::FOLLOWS},
	  {"Follows*", SimpleProgram::DesignAbstraction::FOLLOWST},
	  {"Calls", SimpleProgram::DesignAbstraction::CALLS},
	  {"Calls*", SimpleProgram::DesignAbstraction::CALLST},
	  {"Next", SimpleProgram::DesignAbstraction::NEXT},
	  {"Next*", SimpleProgram::DesignAbstraction::NEXTT},
	  {"Affects", SimpleProgram::DesignAbstraction::AFFECTS}};

  auto tokenValue = suchThatClauseToken->getValue();
  return designAbstractionMap[tokenValue];
}

bool SuchThatClauseParser::isNotClause(std::vector<std::shared_ptr<QueryToken>> &suchThatClauseTokens, bool isAnd) {
  auto firstTokenAfterSuchThat = suchThatClauseTokens[isAnd ? 1 : 2];
  if (firstTokenAfterSuchThat->getValue() == "not") {
	return true;
  }
  return false;
}

PQL::Clause SuchThatClauseParser::parse(
	std::vector<std::shared_ptr<QueryToken>> &suchThatClauseTokens, bool isAnd) {
  std::vector<PQL::Synonym> args;

  bool isNot = isNotClause(suchThatClauseTokens, isAnd);
  int suchThatClauseTokenPos = -1;
  if (isNot) {
	if (isAnd) {
	  suchThatClauseTokenPos = 2;
	} else {
	  suchThatClauseTokenPos = 3;
	}
  } else {
	if (isAnd) {
	  suchThatClauseTokenPos = 1;
	} else {
	  suchThatClauseTokenPos = 2;
	}
  }

  auto suchThatClauseToken = suchThatClauseTokens[suchThatClauseTokenPos];
  auto suchThatClauseType = getSuchThatClauseType(suchThatClauseToken);

  std::vector<std::shared_ptr<QueryToken>> cleanedSuchThatTokens =
	  QueryEvaluator::ParseUtils::removeBracketsAndCommas(
		  suchThatClauseTokens);
  std::vector<std::shared_ptr<QueryToken>> suchThatArgsTokens =
	  QueryEvaluator::ParseUtils::splitTokens(cleanedSuchThatTokens, suchThatClauseTokenPos + 1,
											  cleanedSuchThatTokens.size());

  if (suchThatClauseType == SimpleProgram::DesignAbstraction::MODIFIESS ||
	  suchThatClauseType == SimpleProgram::DesignAbstraction::USESS) {
	auto lArgToken = suchThatArgsTokens[0];
	auto rArgToken = suchThatArgsTokens[1];

	validator->validateUsesModifiesArgs(suchThatArgsTokens);

	SimpleProgram::DesignEntity lArgEntityType;
	SimpleProgram::DesignEntity rArgEntityType;

	if (validator->isSynonym(lArgToken)) {
	  lArgEntityType = QueryEvaluator::ParseUtils::getEntityType(
		  lArgToken, declarations);
	} else {
	  if (lArgToken->getType() == QPS::TokenType::CONSTANT_STRING) {
		lArgEntityType = SimpleProgram::DesignEntity::IDENT;
	  } else if (lArgToken->getType() == QPS::TokenType::INTEGER) {
		lArgEntityType = SimpleProgram::DesignEntity::STMT_NO;
	  } else {
		lArgEntityType = SimpleProgram::DesignEntity::WILDCARD;
	  }
	}

	if (validator->isSynonym(rArgToken)) {
	  rArgEntityType = QueryEvaluator::ParseUtils::getEntityType(
		  rArgToken, declarations);
	} else {
	  if (rArgToken->getType() == QPS::TokenType::CONSTANT_STRING) {
		rArgEntityType = SimpleProgram::DesignEntity::IDENT;
	  } else {
		rArgEntityType = SimpleProgram::DesignEntity::WILDCARD;
	  }
	}

	auto lArg = QueryEvaluator::ParseUtils::createSynonym(lArgEntityType,
														  lArgToken);
	auto rArg = QueryEvaluator::ParseUtils::createSynonym(rArgEntityType,
														  rArgToken);

	bool isUsesModifiesPLArg = lArgEntityType == SimpleProgram::DesignEntity::PROCEDURE
		|| lArgToken->getType() == QPS::TokenType::CONSTANT_STRING && validator->isValidIdent(lArgToken);

	if (suchThatClauseType == SimpleProgram::DesignAbstraction::MODIFIESS) {
	  if (isUsesModifiesPLArg) {
		suchThatClauseType =
			SimpleProgram::DesignAbstraction::MODIFIESP;
	  }
	} else {
	  if (isUsesModifiesPLArg) {
		suchThatClauseType = SimpleProgram::DesignAbstraction::USESP;
	  }
	}

	args.push_back(lArg);
	args.push_back(rArg);
  } else if (suchThatClauseType == SimpleProgram::DesignAbstraction::PARENT ||
	  suchThatClauseType ==
		  SimpleProgram::DesignAbstraction::PARENTT ||
	  suchThatClauseType ==
		  SimpleProgram::DesignAbstraction::FOLLOWS ||
	  suchThatClauseType ==
		  SimpleProgram::DesignAbstraction::FOLLOWST ||
	  suchThatClauseType == SimpleProgram::DesignAbstraction::NEXT ||
	  suchThatClauseType == SimpleProgram::DesignAbstraction::NEXTT ||
	  suchThatClauseType ==
		  SimpleProgram::DesignAbstraction::AFFECTS) {
	auto lArgToken = suchThatArgsTokens[0];
	auto rArgToken = suchThatArgsTokens[1];

	validator->validateFollowsParentsArgs(suchThatArgsTokens);

	SimpleProgram::DesignEntity lArgEntityType;
	SimpleProgram::DesignEntity rArgEntityType;

	if (validator->isSynonym(lArgToken)) {
	  lArgEntityType = QueryEvaluator::ParseUtils::getEntityType(
		  lArgToken, declarations);
	} else {
	  if (lArgToken->getType() == QPS::TokenType::INTEGER) {
		lArgEntityType = SimpleProgram::DesignEntity::STMT_NO;
	  } else {
		lArgEntityType = SimpleProgram::DesignEntity::WILDCARD;
	  }
	}
	if (validator->isSynonym(rArgToken)) {
	  rArgEntityType = QueryEvaluator::ParseUtils::getEntityType(
		  rArgToken, declarations);
	} else {
	  if (rArgToken->getType() == QPS::TokenType::INTEGER) {
		rArgEntityType = SimpleProgram::DesignEntity::STMT_NO;
	  } else {
		rArgEntityType = SimpleProgram::DesignEntity::WILDCARD;
	  }
	}
	auto lArg = QueryEvaluator::ParseUtils::createSynonym(lArgEntityType,
														  lArgToken);
	auto rArg = QueryEvaluator::ParseUtils::createSynonym(rArgEntityType,
														  rArgToken);

	args.push_back(lArg);
	args.push_back(rArg);
  } else if (suchThatClauseType == SimpleProgram::DesignAbstraction::CALLS ||
	  suchThatClauseType == SimpleProgram::DesignAbstraction::CALLST) {
	auto lArgToken = suchThatArgsTokens[0];
	auto rArgToken = suchThatArgsTokens[1];

	validator->validateCallsArgs(suchThatArgsTokens);

	SimpleProgram::DesignEntity lArgEntityType;
	SimpleProgram::DesignEntity rArgEntityType;

	if (validator->isSynonym(lArgToken)) {
	  lArgEntityType = QueryEvaluator::ParseUtils::getEntityType(
		  lArgToken, declarations);
	} else {
	  if (lArgToken->getType() == QPS::TokenType::CONSTANT_STRING) {
		lArgEntityType = SimpleProgram::DesignEntity::IDENT;
	  } else {
		lArgEntityType = SimpleProgram::DesignEntity::WILDCARD;
	  }
	}

	if (validator->isSynonym(rArgToken)) {
	  rArgEntityType = QueryEvaluator::ParseUtils::getEntityType(
		  rArgToken, declarations);
	} else {
	  if (rArgToken->getType() == QPS::TokenType::CONSTANT_STRING) {
		rArgEntityType = SimpleProgram::DesignEntity::IDENT;
	  } else {
		rArgEntityType = SimpleProgram::DesignEntity::WILDCARD;
	  }
	}

	auto lArg = QueryEvaluator::ParseUtils::createSynonym(lArgEntityType,
														  lArgToken);
	auto rArg = QueryEvaluator::ParseUtils::createSynonym(rArgEntityType,
														  rArgToken);

	args.push_back(lArg);
	args.push_back(rArg);
  } else {
	throw QuerySyntaxError("Syntax Error: Invalid such that clause");
  }
  return {suchThatClauseType, args};
}