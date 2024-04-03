#include "ClauseValidator.h"

#include <memory>
#include <string>
#include <unordered_set>

#include "QPS/QuerySyntaxError.h"
#include "QPS/Utils/ParseUtils.h"

ClauseValidator::ClauseValidator(std::vector<PQL::Synonym> &declarations)
	: declarations(declarations), hasSemanticError(false) {}

bool ClauseValidator::isSynonym(std::shared_ptr<QueryToken> token) {
  return token->getType() == QPS::TokenType::NAME;
}

bool ClauseValidator::isValidIdent(std::shared_ptr<QueryToken> token) {
  return token->getType() == QPS::TokenType::CONSTANT_STRING &&
	  QueryEvaluator::ParseUtils::isName(token->getValue());
}

bool ClauseValidator::isStmtRef(std::shared_ptr<QueryToken> token) {
  auto tokenType = token->getType();
  return tokenType == QPS::TokenType::WILDCARD ||
	  tokenType == QPS::TokenType::INTEGER || isSynonym(token);
}

void ClauseValidator::validateStmtRef(std::shared_ptr<QueryToken> token) {
  if (!isStmtRef(token)) {
	throw QuerySyntaxError("Syntax Error: Invalid statement reference");
  }

  if (!isSynonym(token)) {
	return;
  }

  std::unordered_set<SimpleProgram::DesignEntity> stmtTypes(
	  {SimpleProgram::DesignEntity::STMT, SimpleProgram::DesignEntity::ASSIGN,
	   SimpleProgram::DesignEntity::CALL, SimpleProgram::DesignEntity::IF,
	   SimpleProgram::DesignEntity::PRINT, SimpleProgram::DesignEntity::READ,
	   SimpleProgram::DesignEntity::WHILE});

  auto entRefSynonymType =
	  QueryEvaluator::ParseUtils::getEntityType(token, declarations);
  bool isValidSynonymType =
	  stmtTypes.find(QueryEvaluator::ParseUtils::getEntityType(
		  token, declarations)) != stmtTypes.end();

  if (!isValidSynonymType) {
	setSemanticError();
  }
}

bool ClauseValidator::isEntRef(std::shared_ptr<QueryToken> token) {
  return token->getType() == QPS::TokenType::WILDCARD ||
	  isValidIdent(token) || isSynonym(token);
}

void ClauseValidator::validateEntRef(std::shared_ptr<QueryToken> token) {
  if (!isEntRef(token)) {
	throw QuerySyntaxError("Syntax Error: Invalid entity reference");
  }

  if (!isSynonym(token)) {
	return;
  }

  std::unordered_set<SimpleProgram::DesignEntity> entTypes(
	  {SimpleProgram::DesignEntity::PROCEDURE,
	   SimpleProgram::DesignEntity::VARIABLE,
	   SimpleProgram::DesignEntity::CONSTANT});

  auto entRefSynonymType =
	  QueryEvaluator::ParseUtils::getEntityType(token, declarations);
  bool isValidSynonymType =
	  entTypes.find(QueryEvaluator::ParseUtils::getEntityType(
		  token, declarations)) != entTypes.end();

  if (!isValidSynonymType) {
	setSemanticError();
  }
}

bool ClauseValidator::isStmtOrEntRef(std::shared_ptr<QueryToken> token) {
  return isStmtRef(token) || isEntRef(token);
}

void ClauseValidator::validateStmtOrEntRef(std::shared_ptr<QueryToken> token) {
  if (!isStmtOrEntRef(token)) {
	throw QuerySyntaxError(
		"Syntax Error: Neither statement or entity reference");
  }
}

bool ClauseValidator::isExpression(std::shared_ptr<QueryToken> token) {
  return token->getType() == QPS::TokenType::EXPRESSION ||
	  token->getType() == QPS::TokenType::CONSTANT_STRING;
}

bool ClauseValidator::isExpressionSpec(
	std::vector<std::shared_ptr<QueryToken>> &tokens) {
  if (tokens.size() == 1) {
	auto token = tokens[0];
	return isExpression(token) ||
		token->getType() == QPS::TokenType::WILDCARD;
  } else {
	if (tokens.size() != 3) {
	  return false;
	}
	auto token1 = tokens[0];
	auto token2 = tokens[1];
	auto token3 = tokens[2];
	return token1->getType() == QPS::TokenType::WILDCARD &&
		token3->getType() == QPS::TokenType::WILDCARD &&
		isExpression(token2);
  }
}

void ClauseValidator::setSemanticError() { hasSemanticError = true; }

bool ClauseValidator::hasError() { return hasSemanticError; }

void ClauseValidator::validateDeclarationExists(
	std::shared_ptr<QueryToken> synonymToken) {
  std::string id = synonymToken->getValue();
  for (auto &declaration : declarations) {
	if (declaration.identity == id) {
	  return;
	}
  }
  setSemanticError();
}

void ClauseValidator::validateDeclarationExists(std::string synonymIdentity) {
  for (auto &declaration : declarations) {
	if (declaration.identity == synonymIdentity) {
	  return;
	}
  }
  setSemanticError();
}

void ClauseValidator::validateHasSelectSynonym(
	std::vector<std::shared_ptr<QueryToken>> &clauseTokens) {
  if (clauseTokens.empty() || clauseTokens.size() == 1) {
	throw QuerySyntaxError("Syntax Error: No select synonym");
  };
}

void ClauseValidator::validateSelectClauseToken(
	std::shared_ptr<QueryToken> selectToken) {
  if (selectToken->getValue() != "Select") {
	throw QuerySyntaxError("Syntax Error: Select clause should come first");
  }
}

void ClauseValidator::validatePatternSynonym(
	std::shared_ptr<QueryToken> &patternToken) {
  std::unordered_set<SimpleProgram::DesignEntity> allowedTypes(
	  {SimpleProgram::DesignEntity::ASSIGN, SimpleProgram::DesignEntity::IF,
	   SimpleProgram::DesignEntity::WHILE});

  if (!isSynonym(patternToken)) {
	throw QuerySyntaxError(
		"Syntax Error: Pattern synonym should be a synonym");
  }

  auto patternEntityType =
	  QueryEvaluator::ParseUtils::getEntityType(patternToken, declarations);

  if (allowedTypes.find(patternEntityType) == allowedTypes.end()) {
	setSemanticError();
  }
}

void ClauseValidator::validatePatternAssign(
	std::vector<std::shared_ptr<QueryToken>> &patternTokens) {
  int n = patternTokens.size();
  auto lArgToken = patternTokens[0];

  validateEntRef(lArgToken);

  if (isSynonym(lArgToken)) {
	validateDeclarationExists(lArgToken);
  }

  auto remainingArgs =
	  QueryEvaluator::ParseUtils::splitTokens(patternTokens, 1, n);

  if (!isExpressionSpec(remainingArgs)) {
	throw QuerySyntaxError(
		"Syntax Error: Second argument of pattern assign should be an "
		"expression spec");
  }
}

void ClauseValidator::validatePatternIf(
	std::vector<std::shared_ptr<QueryToken>> &patternTokens) {
  for (int count = 0; count < patternTokens.size(); count++) {
	auto currToken = patternTokens[count];
	if (count == 0) {
	  validateEntRef(currToken);

	  if (isSynonym(currToken)) {
		auto argEntityType =
			QueryEvaluator::ParseUtils::getEntityType(currToken, declarations);
		if (argEntityType != SimpleProgram::DesignEntity::VARIABLE) {
		  setSemanticError();
		}
		validateDeclarationExists(currToken);
	  }
	} else if (count == 1 || count == 2) {
	  if (currToken->getType() != QPS::TokenType::WILDCARD) {
		throw QuerySyntaxError(
			"Syntax Error: Second argument and third argument of "
			"pattern if should be a wildcard");
	  }
	} else {
	  throw QuerySyntaxError(
		  "Syntax Error: Pattern if should have three arguments");
	}
  }
}

void ClauseValidator::validatePatternWhile(
	std::vector<std::shared_ptr<QueryToken>> &patternTokens) {
  for (int count = 0; count < patternTokens.size(); count++) {
	auto currToken = patternTokens[count];
	if (count == 0) {
	  validateEntRef(currToken);

	  if (isSynonym(currToken)) {
		auto argEntityType =
			QueryEvaluator::ParseUtils::getEntityType(currToken, declarations);
		if (argEntityType != SimpleProgram::DesignEntity::VARIABLE) {
		  setSemanticError();
		}
		validateDeclarationExists(currToken);
	  }
	} else if (count == 1) {
	  if (currToken->getType() != QPS::TokenType::WILDCARD) {
		throw QuerySyntaxError(
			"Syntax Error: Second argument of pattern while should be "
			"a wildcard");
	  }
	} else {
	  throw QuerySyntaxError(
		  "Syntax Error: Pattern while should have two arguments");
	}
  }
}

void ClauseValidator::validatePatternArgs(
	SimpleProgram::DesignAbstraction patternType,
	std::vector<std::shared_ptr<QueryToken>> &patternArgs) {
  if (patternType == SimpleProgram::DesignAbstraction::PATTERN_ASSIGN) {
	validatePatternAssign(patternArgs);
  } else if (patternType == SimpleProgram::DesignAbstraction::PATTERN_WHILE) {
	validatePatternWhile(patternArgs);
  } else {
	validatePatternIf(patternArgs);
  }
}

void ClauseValidator::validateUsesModifiesArgs(
	std::vector<std::shared_ptr<QueryToken>> &relationshipArgs) {
  if (relationshipArgs.size() != 2) {
	throw QuerySyntaxError(
		"Syntax Error: Uses/Modifies clause should have two arguments");
  }

  auto lArgToken = relationshipArgs[0];

  if (lArgToken->getType() == QPS::TokenType::WILDCARD) {
	setSemanticError();
  }

  auto rArgToken = relationshipArgs[1];

  validateStmtOrEntRef(lArgToken);
  validateEntRef(rArgToken);

  if (isSynonym(lArgToken)) {
	auto lArgEntityType =
		QueryEvaluator::ParseUtils::getEntityType(lArgToken, declarations);
	if (lArgEntityType == SimpleProgram::DesignEntity::VARIABLE ||
		lArgEntityType == SimpleProgram::DesignEntity::CONSTANT) {
	  setSemanticError();
	}
	validateDeclarationExists(lArgToken);
  }

  if (isSynonym(rArgToken)) {
	if (QueryEvaluator::ParseUtils::getEntityType(rArgToken,
												  declarations) !=
		SimpleProgram::DesignEntity::VARIABLE) {
	  setSemanticError();
	}
	validateDeclarationExists(rArgToken);
  }
}

void ClauseValidator::validateFollowsParentsArgs(
	std::vector<std::shared_ptr<QueryToken>> &relationshipArgs) {
  if (relationshipArgs.size() != 2) {
	throw QuerySyntaxError(
		"Syntax Error: Follows/Parent clause should have two arguments");
  }

  auto lArgToken = relationshipArgs[0];
  auto rArgToken = relationshipArgs[1];

  validateStmtRef(lArgToken);

  if (isSynonym(lArgToken)) {
	validateDeclarationExists(lArgToken);
  }

  validateStmtRef(rArgToken);

  if (isSynonym(rArgToken)) {
	validateDeclarationExists(rArgToken);
  }
}

void ClauseValidator::validateCallsArgs(
	std::vector<std::shared_ptr<QueryToken>> &relationshipArgs) {
  if (relationshipArgs.size() != 2) {
	throw QuerySyntaxError(
		"Syntax Error: Calls/Calls* clause should have two arguments");
  }

  auto lArgToken = relationshipArgs[0];
  auto rArgToken = relationshipArgs[1];

  validateEntRef(lArgToken);

  if (isSynonym(lArgToken)) {
	auto lArgEntityType =
		QueryEvaluator::ParseUtils::getEntityType(lArgToken, declarations);
	if (lArgEntityType != SimpleProgram::DesignEntity::PROCEDURE) {
	  setSemanticError();
	}
	validateDeclarationExists(lArgToken);
  }

  validateEntRef(rArgToken);

  if (isSynonym(rArgToken)) {
	auto rArgEntityType =
		QueryEvaluator::ParseUtils::getEntityType(rArgToken, declarations);
	if (rArgEntityType != SimpleProgram::DesignEntity::PROCEDURE) {
	  setSemanticError();
	}
	validateDeclarationExists(rArgToken);
  }
}
