#include "ClauseParser.h"

#include <memory>
#include <string>

#include "ClauseValidator.h"
#include "PatternClauseParser.h"
#include "QPS/QuerySyntaxError.h"
#include "QPS/QueryToken.h"
#include "QPS/Utils/ParseUtils.h"
#include "SuchThatClauseParser.h"
#include "WithClauseParser.h"

ClauseParser::ClauseParser(
	std::vector<std::shared_ptr<QueryToken>> &clauseTokens,
	std::vector<PQL::Synonym> &declarations,
	std::shared_ptr<ClauseValidator> &validator)
	: clauseTokens(clauseTokens),
	  declarations(declarations),
	  validator(validator) {}

PQL::Synonym ClauseParser::getDeclarationUsed(
	std::shared_ptr<QueryToken> synonymToken) {
  std::string id = synonymToken->getValue();
  return getDeclarationUsed(id);
}

PQL::Synonym ClauseParser::getDeclarationUsed(std::string synonymIdentity) {
  for (auto &declaration : declarations) {
	if (declaration.identity == synonymIdentity) {
	  return declaration;
	}
  }
  return PQL::Synonym::createInvalidSynonym();
}

std::vector<PQL::Synonym> ClauseParser::parseSelectClause() {
  ClauseValidator::validateHasSelectSynonym(clauseTokens);
  std::vector<PQL::Synonym> selectSynonyms;
  auto selectToken = clauseTokens[0];
  validator->validateSelectClauseToken(selectToken);
  auto nextToken = clauseTokens[1];
  if (nextToken->getType() == QPS::TokenType::TUPLE) {
	auto tupleSynonyms = QueryEvaluator::ParseUtils::splitTuple(nextToken);
	for (auto &tupleSynonym : tupleSynonyms) {
	  validator->validateDeclarationExists(tupleSynonym);
	  auto declarationUsed = getDeclarationUsed(tupleSynonym);
	  SimpleProgram::DesignEntity entityType = declarationUsed.entityType;
	  auto synonym = QueryEvaluator::ParseUtils::createSynonym(
		  entityType, tupleSynonym);
	  selectSynonyms.push_back(synonym);
	}
  } else {
	// DO FOR BOOLEAN HERE
	// BOOLEAN SHOULD CHECK IF A DECLARATION EXIST, NOT VALIDATE. IF EXIST
	// THEN VALIDATE? IF NOT EXIST THEN CALL BOOLEAN QUERY
	validator->validateDeclarationExists(nextToken);
	auto declarationUsed = getDeclarationUsed(nextToken);
	SimpleProgram::DesignEntity entityType = declarationUsed.entityType;
	auto synonym =
		QueryEvaluator::ParseUtils::createSynonym(entityType, nextToken);
	selectSynonyms.push_back(synonym);
  }

  return selectSynonyms;
}

std::vector<PQL::Clause> ClauseParser::parseRelationshipClause() {
  std::vector<PQL::Clause> clauses;

  auto relationshipClauseTokens =
	  std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(
		  QueryEvaluator::ParseUtils::splitTokens(clauseTokens, 2,
												  clauseTokens.size()));
  PatternClauseParser patternClauseParser =
	  PatternClauseParser(relationshipClauseTokens, validator, declarations);
  SuchThatClauseParser suchThatClauseParser =
	  SuchThatClauseParser(relationshipClauseTokens, validator, declarations);
  WithClauseParser withClauseParser = WithClauseParser(relationshipClauseTokens, validator, declarations);

  std::string prevClauseToken;
  bool isAnd = false;

  while (!relationshipClauseTokens->empty()) {
	std::shared_ptr<QueryToken> currToken = relationshipClauseTokens->at(0);
	std::string tokenValue = currToken->getValue();

	if (tokenValue == "and") {
	  isAnd = true;
	  tokenValue = prevClauseToken;
	}

	prevClauseToken = tokenValue;

	if (tokenValue == "pattern") {
	  auto patternClauseTokens = patternClauseParser.getPatternClause();
	  PQL::Clause patternClause =
		  patternClauseParser.parse(patternClauseTokens);
	  clauses.push_back(patternClause);
	} else if (tokenValue == "such") {
	  auto suchThatClauseTokens =
		  suchThatClauseParser.getSuchThatClause();
	  PQL::Clause suchThatClause =
		  suchThatClauseParser.parse(suchThatClauseTokens, isAnd);
	  clauses.push_back(suchThatClause);
	} else if (tokenValue == "with") {
	  auto withClauseTokens = withClauseParser.getWithClause();
	  PQL::Clause withClause = withClauseParser.parse(withClauseTokens);
	  clauses.push_back(withClause);
	} else {
	  throw QuerySyntaxError("Syntax Error: Invalid relationship clause");
	}
	isAnd = false;
  }

  return clauses;
}