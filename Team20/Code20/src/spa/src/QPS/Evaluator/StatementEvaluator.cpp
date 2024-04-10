#include "StatementEvaluator.h"
#include "ClauseEvaluator.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


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
	  || clause.arguments[1].entityType == SimpleProgram::DesignEntity::WILDCARD
	  || clause.clauseType == SimpleProgram::DesignAbstraction::NEXTT) {
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
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::FOLLOWS,
	   [this, leftStmtNum, rightStmtNum] { return reader->containsFollowsRelationship(leftStmtNum, rightStmtNum); }},
	  {SimpleProgram::DesignAbstraction::FOLLOWST,
	   [this, leftStmtNum, rightStmtNum] { return reader->containsFollowsTRelationship(leftStmtNum, rightStmtNum); }},
	  {SimpleProgram::DesignAbstraction::PARENT,
	   [this, leftStmtNum, rightStmtNum] { return reader->containsParentRelationship(leftStmtNum, rightStmtNum); }},
	  {SimpleProgram::DesignAbstraction::PARENTT,
	   [this, leftStmtNum, rightStmtNum] { return reader->containsParentTRelationship(leftStmtNum, rightStmtNum); }},
	  {SimpleProgram::DesignAbstraction::NEXT,
	   [this, leftStmtNum, rightStmtNum] { return reader->containsNextRelationship(leftStmtNum, rightStmtNum); }},
	  {SimpleProgram::DesignAbstraction::NEXTT,
	   [this, leftStmtNum, rightStmtNum] { return reader->containsNextTRelationship(leftStmtNum, rightStmtNum); }},
	  {SimpleProgram::DesignAbstraction::AFFECTS,
	   [this, leftStmtNum, rightStmtNum] { return reader->containsAffectsRelationship(leftStmtNum, rightStmtNum); }},
  };

  if (funcMap.find(relationship) == funcMap.end()) {
	return false;
  }
  return funcMap[relationship]();
}

bool StatementEvaluator::hasAtLeastOneRelationship() {
  // handles XXX(_, _)
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::FOLLOWS, [this] { return reader->hasFollowsRelationship(); }},
	  {SimpleProgram::DesignAbstraction::FOLLOWST, [this] { return reader->hasFollowsTRelationship(); }},
	  {SimpleProgram::DesignAbstraction::PARENT, [this] { return reader->hasParentRelationship(); }},
	  {SimpleProgram::DesignAbstraction::PARENTT, [this] { return reader->hasParentTRelationship(); }},
	  {SimpleProgram::DesignAbstraction::NEXT, [this] { return reader->hasNextRelationship(); }},
	  {SimpleProgram::DesignAbstraction::NEXTT, [this] { return reader->hasNextTRelationship(); }},
	  {SimpleProgram::DesignAbstraction::AFFECTS, [this] { return reader->hasAffectsRelationship(); }},
  };

  bool isNotEmpty = false;
  if (funcMap.find(clause.clauseType) != funcMap.end()) {
	isNotEmpty = funcMap[clause.clauseType]();
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

	std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<int>()>> funcMap = {
		{SimpleProgram::DesignAbstraction::FOLLOWS, [this, leftStmtNum] { return reader->getFollows(leftStmtNum); }},
		{SimpleProgram::DesignAbstraction::FOLLOWST,
		 [this, leftStmtNum] { return reader->getFollowsT(leftStmtNum); }},
		{SimpleProgram::DesignAbstraction::PARENT, [this, leftStmtNum] { return reader->getChild(leftStmtNum); }},
		{SimpleProgram::DesignAbstraction::PARENTT, [this, leftStmtNum] { return reader->getChildT(leftStmtNum); }},
		{SimpleProgram::DesignAbstraction::NEXT, [this, leftStmtNum] { return reader->getNext(leftStmtNum); }},
		{SimpleProgram::DesignAbstraction::NEXTT, [this, leftStmtNum] { return reader->getNextT(leftStmtNum); }},
		{SimpleProgram::DesignAbstraction::AFFECTS, [this, leftStmtNum] { return reader->getAffects(leftStmtNum); }},
	};

	std::vector<int> rResults;
	if (funcMap.find(clause.clauseType) != funcMap.end()) {
	  rResults = funcMap[clause.clauseType]();
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

	std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<int>()>> funcMap = {
		{SimpleProgram::DesignAbstraction::FOLLOWS,
		 [this, rightStmtNum] { return reader->getFollowing(rightStmtNum); }},
		{SimpleProgram::DesignAbstraction::FOLLOWST,
		 [this, rightStmtNum] { return reader->getFollowingT(rightStmtNum); }},
		{SimpleProgram::DesignAbstraction::PARENT, [this, rightStmtNum] { return reader->getParent(rightStmtNum); }},
		{SimpleProgram::DesignAbstraction::PARENTT, [this, rightStmtNum] { return reader->getParentT(rightStmtNum); }},
		{SimpleProgram::DesignAbstraction::NEXT, [this, rightStmtNum] { return reader->getNextReverse(rightStmtNum); }},
		{SimpleProgram::DesignAbstraction::NEXTT,
		 [this, rightStmtNum] { return reader->getNextTReverse(rightStmtNum); }},
		{SimpleProgram::DesignAbstraction::AFFECTS,
		 [this, rightStmtNum] { return reader->getAffectsReverse(rightStmtNum); }},
	};

	std::vector<int> lResults;
	if (funcMap.find(clause.clauseType) != funcMap.end()) {
	  lResults = funcMap[clause.clauseType]();
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
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<int>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::FOLLOWS, [this] { return reader->getFolloweeStmts(); }},
	  {SimpleProgram::DesignAbstraction::FOLLOWST, [this] { return reader->getFolloweeTStmts(); }},
	  {SimpleProgram::DesignAbstraction::PARENT, [this] { return reader->getParentStmts(); }},
	  {SimpleProgram::DesignAbstraction::PARENTT, [this] { return reader->getParentTStmts(); }},
	  {SimpleProgram::DesignAbstraction::NEXT, [this] { return reader->getNextReverse(); }},
	  {SimpleProgram::DesignAbstraction::NEXTT, [this] { return reader->getNextTReverse(); }},
	  {SimpleProgram::DesignAbstraction::AFFECTS, [this] { return reader->getAffectsReverse(); }},
  };

  std::vector<int> keyStmtNums;
  if (funcMap.find(clause.clauseType) != funcMap.end()) {
	keyStmtNums = funcMap[clause.clauseType]();
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
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<int>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::FOLLOWS, [this] { return reader->getFollowerStmts(); }},
	  {SimpleProgram::DesignAbstraction::FOLLOWST, [this] { return reader->getFollowerTStmts(); }},
	  {SimpleProgram::DesignAbstraction::PARENT, [this] { return reader->getChildStmts(); }},
	  {SimpleProgram::DesignAbstraction::PARENTT, [this] { return reader->getChildTStmts(); }},
	  {SimpleProgram::DesignAbstraction::NEXT, [this] { return reader->getNext(); }},
	  {SimpleProgram::DesignAbstraction::NEXTT, [this] { return reader->getNextT(); }},
	  {SimpleProgram::DesignAbstraction::AFFECTS, [this] { return reader->getAffects(); }},
  };

  std::vector<int> valueStmtNums;
  if (funcMap.find(clause.clauseType) != funcMap.end()) {
	valueStmtNums = funcMap[clause.clauseType]();
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