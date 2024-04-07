#include <string>
#include <unordered_set>
#include <vector>

#include "WithEvaluator.h"

namespace QueryEvaluator {

bool WithEvaluator::evaluate() {
  if (isAlwaysTrue()) {
	// return true if not isNegated, false if isNegated, opposite of isNegated
	return !clause.isNegated;
  }

  if (isAlwaysFalse()) {
	// return false if not isNegated, true if isNegated, same as value of isNegated
	return clause.isNegated;
  }

  // the two on top should have already handled both non-AttrRef
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  if (lArg.attribute != SimpleProgram::AttributeRef::NO_REF && rArg.attribute != SimpleProgram::AttributeRef::NO_REF) {
	// both are procName/varName/stmt
	return handleDoubleAttrRef();
  } else if (lArg.attribute == SimpleProgram::AttributeRef::NO_REF
	  || rArg.attribute == SimpleProgram::AttributeRef::NO_REF) {
	// only one is procName/varName/stmt
	return handleSingleAttrRef();
  }

  return false;
}

bool WithEvaluator::isAlwaysFalse() {
  // handles lArg.entityType == rArg.entityType == IDENT/VALUE
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  if (lArg.attribute != SimpleProgram::AttributeRef::NO_REF
	  || rArg.attribute != SimpleProgram::AttributeRef::NO_REF) {
	// at least one of the arguments is procName/varName/stmt
	return false;
  }

  // both are either NAME/VALUE
  if (lArg.entityType != rArg.entityType) {
	// NAME = VALUE / VALUE = NAME
	return true;
  }

  // NAME = NAME / VALUE = VALUE, check for the actual value
  return !(lArg.identity == rArg.identity);
}

bool WithEvaluator::isAlwaysTrue() {
  // if lArg == rArg it is always evaluated to true
  // e.g. 20 = 20, p.procName = p.procName, TEST_IDENT = TEST_IDENT
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  return lArg == rArg;
}

bool WithEvaluator::handleSingleAttrRef() {
  // (AttrRef, IDENT/VALUE) / (IDENT/VALUE, AttrRef)

  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  if (lArg.entityType == SimpleProgram::DesignEntity::IDENT
	  || lArg.entityType == SimpleProgram::DesignEntity::INTEGER) {
	// swap lArg and rArg to make lArg the AttrRef
	PQL::Synonym temp = lArg;
	lArg = rArg;
	rArg = temp;
  }

  if (hasMultiAttrRef(lArg) && rArg.entityType == SimpleProgram::DesignEntity::IDENT) {
	// CALL/READ/PRINT & IDENT, NEED TO QUERY PKB TO GET THE RESULT
	std::vector<int> stmtNums;
	switch (lArg.entityType) {
	  case SimpleProgram::DesignEntity::CALL:
		stmtNums = reader->getCallsProcStmtNum(rArg.identity);
		break;
	  case SimpleProgram::DesignEntity::READ:
		stmtNums = reader->getReadStmtNum(rArg.identity);
		break;
	  case SimpleProgram::DesignEntity::PRINT:
		stmtNums = reader->getPrintStmtNum(rArg.identity);
		break;
	  default:
		stmtNums = {};
	}

	if (clause.isNegated) {
	  stmtNums = negateIntResults(lArg, stmtNums);
	}
	if (stmtNums.empty()) {
	  return false;
	}

	resultStore->createColumn(lArg, stmtNums);
	return true;
  }

  if (hasIntegerAttrRef(lArg) && rArg.entityType == SimpleProgram::DesignEntity::INTEGER) {
	std::vector<int> values = {stoi(rArg.identity)};
	if (clause.isNegated) {
	  values = negateIntResults(lArg, values);
	}

	if (values.empty()) {
	  return false;
	}
	resultStore->createColumn(lArg, values);
	return true;
  }

  if (hasNameAttrRef(lArg) && rArg.entityType == SimpleProgram::DesignEntity::IDENT) {
	std::vector<std::string> values = {rArg.identity};
	if (clause.isNegated) {
	  values = negateStringResults(lArg, values);
	}

	if (values.empty()) {
	  return false;
	}
	resultStore->createColumn(lArg, values);
	return true;
  }

  return false;
}

bool WithEvaluator::handleDoubleAttrRef() {
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  if (!canCompare(lArg, rArg)) {
	return false;
  }

  if (lArg.attribute == SimpleProgram::AttributeRef::INTEGER) {
	std::vector<int> lValues = ClauseEvaluator::getAllIntResults(lArg);
	std::vector<int> rValues = ClauseEvaluator::getAllIntResults(rArg);
	return createDoubleColumnResult(lArg, rArg, lValues, rValues);
  } else if (lArg.attribute == SimpleProgram::AttributeRef::NAME) {
	std::vector<std::string> lValues = getIdentValues(lArg);
	std::vector<std::string> rValues = getIdentValues(rArg);
	return createDoubleColumnResult(lArg, rArg, lValues, rValues);
  }
  return false;
}

bool WithEvaluator::hasIntegerAttrRef(const PQL::Synonym &syn) {
  if (syn.entityType == SimpleProgram::DesignEntity::CONSTANT
	  || syn.entityType == SimpleProgram::DesignEntity::STMT
	  || syn.entityType == SimpleProgram::DesignEntity::READ
	  || syn.entityType == SimpleProgram::DesignEntity::PRINT
	  || syn.entityType == SimpleProgram::DesignEntity::CALL
	  || syn.entityType == SimpleProgram::DesignEntity::WHILE
	  || syn.entityType == SimpleProgram::DesignEntity::IF
	  || syn.entityType == SimpleProgram::DesignEntity::ASSIGN) {
	return syn.attribute == SimpleProgram::AttributeRef::INTEGER;
  }

  return false;
}

bool WithEvaluator::hasNameAttrRef(const PQL::Synonym &syn) {
  if (syn.entityType == SimpleProgram::DesignEntity::PROCEDURE
	  || syn.entityType == SimpleProgram::DesignEntity::CALL
	  || syn.entityType == SimpleProgram::DesignEntity::VARIABLE
	  || syn.entityType == SimpleProgram::DesignEntity::READ
	  || syn.entityType == SimpleProgram::DesignEntity::PRINT) {
	return syn.attribute == SimpleProgram::AttributeRef::NAME;
  }

  return false;
}

bool WithEvaluator::hasMultiAttrRef(const PQL::Synonym &syn) {
  if (syn.entityType == SimpleProgram::DesignEntity::CALL
	  || syn.entityType == SimpleProgram::DesignEntity::READ
	  || syn.entityType == SimpleProgram::DesignEntity::PRINT) {
	// call have procName/stmt, read & print have varName/stmt
	return true;
  }

  return false;
}

bool WithEvaluator::canCompare(const PQL::Synonym &lArg, const PQL::Synonym &rArg) {
  return lArg.attribute == rArg.attribute;
}

std::vector<std::string> WithEvaluator::getIdentValues(const PQL::Synonym &syn) {
  switch (syn.entityType) {
	case SimpleProgram::DesignEntity::PROCEDURE:
	  return reader->getAllProcedures();
	case SimpleProgram::DesignEntity::CALL:
	  return reader->getCallsProcName();
	case SimpleProgram::DesignEntity::VARIABLE:
	  return reader->getAllVariables();
	case SimpleProgram::DesignEntity::READ:
	  return reader->getReadVariable();
	case SimpleProgram::DesignEntity::PRINT:
	  return reader->getPrintVariable();
	default:
	  return {};
  }
}

template<typename T>
bool WithEvaluator::createDoubleColumnResult(const PQL::Synonym &lArg,
											 const PQL::Synonym &rArg,
											 std::vector<T> &lValues,
											 std::vector<T> &rValues) {
  if (lValues.empty() || rValues.empty()) {
	return false;
  }

  std::vector<T> intersection;
  if constexpr (std::is_same_v<T, int>) {
	intersection = ClauseEvaluator::getIntersection(lValues, rValues);
  } else if constexpr (std::is_same_v<T, std::string>) {
	intersection = getStringIntersection(lValues, rValues);
  } else {
	return false;
  }

  if (clause.isNegated) {
	lValues = negateResults(lArg, intersection);
	rValues = negateResults(rArg, intersection);
  } else {
	lValues = intersection;
	rValues = intersection;
  }

  if (lValues.empty() || rValues.empty()) {
	return false;
  }

  resultStore->createColumn(lArg, lValues);
  resultStore->createColumn(rArg, rValues);
  return true;
}

std::vector<std::string> WithEvaluator::getStringIntersection(std::vector<std::string> &v1,
															  std::vector<std::string> &v2) {
  std::unordered_set<std::string> stringSet = std::unordered_set<std::string>(v1.begin(), v1.end());
  std::vector<std::string> res;

  for (auto &v : v2) {
	if (stringSet.find(v) != stringSet.end()) {
	  res.push_back(v);
	}
  }
  return res;
}

std::vector<std::string> WithEvaluator::negateStringResults(const PQL::Synonym &syn,
															const std::vector<std::string> &selected) {
  std::vector<std::string> stringResults = getIdentValues(syn);
  std::vector<std::string> isNegatedResults;
  std::unordered_set<std::string> selectedSet = std::unordered_set<std::string>(selected.begin(), selected.end());

  for (auto const &val : stringResults) {
	if (selectedSet.find(val) == selectedSet.end()) {
	  isNegatedResults.push_back(val);
	}
  }

  return isNegatedResults;
}

template<typename T>
std::vector<T> WithEvaluator::negateResults(const PQL::Synonym &syn, const std::vector<T> &selected) {
  if constexpr (std::is_same_v<T, int>) {
	return negateIntResults(syn, selected);
  } else if constexpr (std::is_same_v<T, std::string>) {
	return negateStringResults(syn, selected);
  }

  return {};
}
}
