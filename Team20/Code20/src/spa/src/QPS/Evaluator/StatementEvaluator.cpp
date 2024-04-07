#include "StatementEvaluator.h"
#include "ClauseEvaluator.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace QueryEvaluator {

bool StatementEvaluator::evaluate() {
  // Handles clauses with only stmtRef: FOLLOWS/FOLLOWS*/PARENT/PARENT*/NEXT/NEXT*

  if (isAlwaysFalse()) {
	// Handles 7 cases: two same stmtNum (2,2) (overlaps with STMT_NO_1, STMT_NO_2), 7 same synonym (Y,Y)
	return false;
  }

  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];

  // Handles 2 cases: (STMT_NO_1, STMT_NO_2), (WILDCARD, WILDCARD)
  if (lArg.entityType == rArg.entityType) {
	if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
	  return hasRelationship();
	} else if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	  return hasAtLeastOneRelationship();
	}
  }

  // Handles 8 cases: 7 * (STMT_NUM, SYN), (STMT_NUM, WILDCARD)
  if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
	return getForwardRelationship();
  }

  // Handles 8 cases: 7 * (SYN, STMT_NUM), (WILDCARD, STMT_NUM)
  if (rArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
	return getReversedRelationship();
  }

  // Handles 7 cases: 7 * (WILDCARD, SYN)
  if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	return getWildcardSynonym();
  }

  // Handles 7 cases: 7 * (SYN, WILDCARD)
  if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	return getSynonymWildcard();
  }

  // Handles 42 cases: 7 * 6 (SYN_A, SYN_B) WHERE SYN_A != SYN_B
  return getDoubleSynonym();
}

bool StatementEvaluator::isAlwaysFalse() {
  if (clause.arguments[0].entityType == SimpleProgram::DesignEntity::WILDCARD
	  || clause.arguments[1].entityType == SimpleProgram::DesignEntity::WILDCARD) {
	return false;
  }

  return clause.arguments[0] == clause.arguments[1];
}

bool StatementEvaluator::hasRelationship() {
  // handles XXX(STMT_NO, STMT_NO)
  int leftStmtNum = stoi(clause.arguments[0].identity);
  int rightStmtNum = stoi(clause.arguments[1].identity);
  bool isNotEmpty = hasRelationship(clause.clauseType, leftStmtNum, rightStmtNum);

  if (clause.isNegated) {
	return !isNotEmpty;
  } else {
	return isNotEmpty;
  }
}

bool StatementEvaluator::hasRelationship(const SimpleProgram::DesignAbstraction &relationship, int leftStmtNum,
										 int rightStmtNum) {
  switch (relationship) {
	case SimpleProgram::DesignAbstraction::FOLLOWS:
	  return reader->containsFollowsRelationship(leftStmtNum,
												 rightStmtNum);
	case SimpleProgram::DesignAbstraction::FOLLOWST:
	  return reader->containsFollowsTRelationship(leftStmtNum,
												  rightStmtNum);
	case SimpleProgram::DesignAbstraction::PARENT:
	  return reader->containsParentRelationship(leftStmtNum, rightStmtNum);
	case SimpleProgram::DesignAbstraction::PARENTT:
	  return reader->containsParentTRelationship(leftStmtNum,
												 rightStmtNum);
	case SimpleProgram::DesignAbstraction::NEXT:
	  return reader->containsNextRelationship(leftStmtNum, rightStmtNum);
	case SimpleProgram::DesignAbstraction::NEXTT:
	  return reader->containsNextTRelationship(leftStmtNum, rightStmtNum);
	case SimpleProgram::DesignAbstraction::AFFECTS:
	  return reader->containsAffectsRelationship(leftStmtNum, rightStmtNum);
	default:
	  // TODO: throw illegal argument, not allowed relationship type for statement only queries
	  return false;
  }
}

bool StatementEvaluator::hasAtLeastOneRelationship() {
  // handles XXX(_, _)
  bool isNotEmpty = false;
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::FOLLOWS:
	  isNotEmpty = reader->hasFollowsRelationship();
	  break;
	case SimpleProgram::DesignAbstraction::FOLLOWST:
	  isNotEmpty = reader->hasFollowsTRelationship();
	  break;
	case SimpleProgram::DesignAbstraction::PARENT:
	  isNotEmpty = reader->hasParentRelationship();
	  break;
	case SimpleProgram::DesignAbstraction::PARENTT:
	  isNotEmpty = reader->hasParentTRelationship();
	  break;
	case SimpleProgram::DesignAbstraction::NEXT:
	  isNotEmpty = reader->hasNextRelationship();
	  break;
	case SimpleProgram::DesignAbstraction::NEXTT:
	  isNotEmpty = reader->hasNextTRelationship();
	  break;
	case SimpleProgram::DesignAbstraction::AFFECTS:
	  isNotEmpty = reader->hasAffectsRelationship();
	  break;
	default:
	  // TODO: throw illegal argument, not allowed relationship type for statement only queries
	  isNotEmpty = false;
  }

  if (clause.isNegated) {
	return !isNotEmpty;
  }

  return isNotEmpty;
}

bool StatementEvaluator::getForwardRelationship() {
  // handles XXX(STMT_NUM, SYNONYM) and (STMT_NUM, WILDCARD)
  PQL::Synonym rArg = clause.arguments[1];
  int leftStmtNum = stoi(clause.arguments[0].identity);

  if (rArg.entityType == SimpleProgram::DesignEntity::STMT
	  || rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {

	std::vector<int> rResults;
	switch (clause.clauseType) {
	  case SimpleProgram::DesignAbstraction::FOLLOWS:
		rResults = reader->getFollows(leftStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::FOLLOWST:
		rResults = reader->getFollowsT(leftStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::PARENT:
		rResults = reader->getChild(leftStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::PARENTT:
		rResults = reader->getChildT(leftStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::NEXT:
		rResults = reader->getNext(leftStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::NEXTT:
		rResults = reader->getNextT(leftStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::AFFECTS:
		rResults = reader->getAffects(leftStmtNum);
		break;
	  default:
		// TODO: throw illegal argument, not allowed relationship type for statement only queries
		rResults = {};
	}

	if (clause.isNegated) {
	  rResults = negateIntResults(rArg, rResults);
	}

	bool isNotEmpty = !rResults.empty();
	if (rArg.entityType == SimpleProgram::DesignEntity::STMT && isNotEmpty) {
	  resultStore->createColumn(rArg, rResults);
	}

	return isNotEmpty;
  }

  // rArg = stmtRef synonyms excluding STMT
  return getRightResults();
}

bool StatementEvaluator::getRightResults() {
  // get all right stmtNums that satisfy the relationship with the fixed left stmtNum
  PQL::Synonym rArg = clause.arguments[1];
  int leftStmtNum = stoi(clause.arguments[0].identity);

  std::vector<int> rResults = {};
  std::vector<int> rightStmtNums = ClauseEvaluator::getAllIntResults(rArg);

  for (auto rightStmtNum : rightStmtNums) {
	if (hasRelationship(clause.clauseType, leftStmtNum, rightStmtNum)) {
	  rResults.push_back(rightStmtNum);
	}
  }

  if (clause.isNegated) {
	rResults = negateIntResults(rArg, rResults);
  }
  bool isNotEmpty = !rResults.empty();
  if (isNotEmpty) {
	resultStore->createColumn(rArg, rResults);
  }

  return isNotEmpty;
}

bool StatementEvaluator::getWildcardSynonym() {
  // handles (_, SYN)
  PQL::Synonym rArg = clause.arguments[1];
  std::vector<int> rResults = getUniqueValues(rArg);

  if (clause.isNegated) {
	rResults = negateIntResults(rArg, rResults);
  }

  if (rResults.empty()) {
	return false;
  }

  resultStore->createColumn(rArg, rResults);
  return true;
}

bool StatementEvaluator::getReversedRelationship() {
  // handles XXX(SYNONYM, STMT_NUM) and (WILDCARD, STMT_NUM)
  PQL::Synonym lArg = clause.arguments[0];
  int rightStmtNum = stoi(clause.arguments[1].identity);

  if (lArg.entityType == SimpleProgram::DesignEntity::STMT
	  || lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {

	std::vector<int> lResults;
	switch (clause.clauseType) {
	  case SimpleProgram::DesignAbstraction::FOLLOWS:
		lResults = reader->getFollowing(rightStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::FOLLOWST:
		lResults = reader->getFollowingT(rightStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::PARENT:
		lResults = reader->getParent(rightStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::PARENTT:
		lResults = reader->getParentT(rightStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::NEXT:
		lResults = reader->getNextReverse(rightStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::NEXTT:
		lResults = reader->getNextTReverse(rightStmtNum);
		break;
	  case SimpleProgram::DesignAbstraction::AFFECTS:
		lResults = reader->getAffectsReverse(rightStmtNum);
		break;
	  default:
		// TODO: throw illegal argument, not allowed relationship type for statement only queries
		lResults = {};
	}

	if (clause.isNegated) {
	  lResults = negateIntResults(lArg, lResults);
	}

	bool isNotEmpty = !lResults.empty();
	if (lArg.entityType == SimpleProgram::DesignEntity::STMT && isNotEmpty) {
	  resultStore->createColumn(lArg, lResults);
	}
	return isNotEmpty;
  }

  return getLeftResults();
}

bool StatementEvaluator::getLeftResults() {
  // get all left stmtNums that satisfy the relationship with the fixed right stmtNum
  PQL::Synonym lArg = clause.arguments[0];
  int rightStmtNum = stoi(clause.arguments[1].identity);

  std::vector<int> lResults = {};
  std::vector<int> leftStmtNums = ClauseEvaluator::getAllIntResults(lArg);

  for (auto leftStmtNum : leftStmtNums) {
	if (hasRelationship(clause.clauseType, leftStmtNum, rightStmtNum)) {
	  lResults.push_back(leftStmtNum);
	}
  }

  if (clause.isNegated) {
	lResults = negateIntResults(lArg, lResults);
  }

  bool isNotEmpty = !lResults.empty();
  if (isNotEmpty) {
	resultStore->createColumn(lArg, lResults);
  }

  return isNotEmpty;
}

bool StatementEvaluator::getSynonymWildcard() {
  // handles (SYN, _)
  PQL::Synonym lArg = clause.arguments[0];
  std::vector<int> lResults = getUniqueKeys(lArg);

  if (clause.isNegated) {
	lResults = negateIntResults(lArg, lResults);
  }

  if (lResults.empty()) {
	return false;
  }

  resultStore->createColumn(lArg, lResults);
  return true;
}

std::vector<int> StatementEvaluator::getUniqueKeys(const PQL::Synonym &syn) {
  // get all possible left stmtNum that satisfies the relationship
  std::vector<int> keyStmtNums;
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::FOLLOWS:
	  keyStmtNums = reader->getFolloweeStmts();
	  break;
	case SimpleProgram::DesignAbstraction::FOLLOWST:
	  keyStmtNums = reader->getFolloweeTStmts();
	  break;
	case SimpleProgram::DesignAbstraction::PARENT:
	  keyStmtNums = reader->getParentStmts();
	  break;
	case SimpleProgram::DesignAbstraction::PARENTT:
	  keyStmtNums = reader->getParentTStmts();
	  break;
	case SimpleProgram::DesignAbstraction::NEXT:
	  keyStmtNums = reader->getNextReverse();
	  break;
	case SimpleProgram::DesignAbstraction::NEXTT:
	  keyStmtNums = reader->getNextTReverse();
	  break;
	case SimpleProgram::DesignAbstraction::AFFECTS:
	  keyStmtNums = reader->getAffects();
	  break;
	default:
	  // TODO: throw illegal argument, not allowed relationship type for statement only queries
	  return {};
  }

  if (syn.entityType == SimpleProgram::DesignEntity::STMT) {
	return keyStmtNums;
  }

  std::vector<int> synStmtNums = ClauseEvaluator::getAllIntResults(syn);
  std::vector<int> intersection = ClauseEvaluator::getIntersection(synStmtNums, keyStmtNums);

  return intersection;
}

std::vector<int> StatementEvaluator::getUniqueValues(const PQL::Synonym &syn) {
  // get all possible right stmtNum that satisfies the relationship
  std::vector<int> valueStmtNums;
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::FOLLOWS:
	  valueStmtNums = reader->getFollowerStmts();
	  break;
	case SimpleProgram::DesignAbstraction::FOLLOWST:
	  valueStmtNums = reader->getFollowerTStmts();
	  break;
	case SimpleProgram::DesignAbstraction::PARENT:
	  valueStmtNums = reader->getChildStmts();
	  break;
	case SimpleProgram::DesignAbstraction::PARENTT:
	  valueStmtNums = reader->getChildTStmts();
	  break;
	case SimpleProgram::DesignAbstraction::NEXT:
	  valueStmtNums = reader->getNext();
	  break;
	case SimpleProgram::DesignAbstraction::NEXTT:
	  valueStmtNums = reader->getNextT();
	  break;
	case SimpleProgram::DesignAbstraction::AFFECTS:
	  valueStmtNums = reader->getAffects();
	  break;
	default:
	  // TODO: throw illegal argument, not allowed relationship type for statement only queries
	  valueStmtNums = {};
  }

  if (syn.entityType == SimpleProgram::DesignEntity::STMT) {
	return valueStmtNums;
  }

  std::vector<int> synStmtNums = ClauseEvaluator::getAllIntResults(syn);
  std::vector<int> intersection = ClauseEvaluator::getIntersection(synStmtNums, valueStmtNums);

  return intersection;
}

bool StatementEvaluator::getDoubleSynonym() {
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  std::vector<int> lValues = getUniqueKeys(lArg);
  std::vector<int> rValues = getUniqueValues(rArg);
  std::vector<std::string> lResults = {};
  std::vector<std::string> rResults = {};

  for (auto v1 : lValues) {
	for (auto v2 : rValues) {
	  if (hasRelationship(clause.clauseType, v1, v2) || clause.isNegated) {
		lResults.push_back(std::to_string(v1));
		rResults.push_back(std::to_string(v2));
	  }
	}
  }

  if (lResults.empty()) {
	return false;
  }

  std::vector<std::vector<std::string>> table = {lResults, rResults};
  std::vector<std::string> colNames = {lArg.identity, rArg.identity};
  std::unordered_map<std::string, size_t> colNameToIndex;
  for (size_t i = 0; i < colNames.size(); i++) {
	colNameToIndex[colNames[i]] = i;
  }
  resultStore->insertResult(std::make_shared<Result>(table, colNames, colNameToIndex));
  return true;
}
}