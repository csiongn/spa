#include "IfAndWhilePatternEvaluator.h"

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

namespace QueryEvaluator {
bool IfAndWhilePatternEvaluator::evaluate() {
  PQL::Synonym lArg = clause.arguments[1];

  if (lArg.entityType == SimpleProgram::DesignEntity::VARIABLE) {
	// (VAR, _)
	return getForwardRelationship();
  } else if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	// (_, _)
	return hasAtLeastOneRelationship();
  } else {
	// ("IDENT", _)
	return hasRelationship();
  }
}

bool IfAndWhilePatternEvaluator::hasRelationship() {
  // handles (IDENT, _) & (IDENT, _, _)
  PQL::Synonym patternSyn = clause.arguments[0];
  PQL::Synonym lArg = clause.arguments[1];

  std::vector<int> stmtNums = getAllStmts(lArg.identity);

  if (clause.isNegated) {
	stmtNums = negateIntResults(lArg, stmtNums);
  }

  if (stmtNums.empty()) {
	return false;
  }

  // add the if/while SYN to result store
  resultStore->createColumn(patternSyn, stmtNums);
  return true;
}

bool IfAndWhilePatternEvaluator::hasAtLeastOneRelationship() {
  // handles (_, _)
  // not adding IF_SYN/WHILE_SYN to result store, result same as all if/while statements
  // already added during initialise SYN in QueryEvaluator
  PQL::Synonym patternSyn = clause.arguments[0];
  bool isNotEmpty = false;
  if (patternSyn.entityType == SimpleProgram::DesignEntity::IF) {
	isNotEmpty = reader->hasIfPattern();
  } else if (patternSyn.entityType == SimpleProgram::DesignEntity::WHILE) {
	isNotEmpty = reader->hasWhilePattern();
  }

  if (clause.isNegated) {
	isNotEmpty = !isNotEmpty;
  }

  return isNotEmpty;
}

bool IfAndWhilePatternEvaluator::getForwardRelationship() {
  // Handles (VAR, _)
  return getSynonymWildcard();
}

bool IfAndWhilePatternEvaluator::getSynonymWildcard() {
  // Handles (VAR, _) / (VAR, _, _)
  return getLeftResults();
}

bool IfAndWhilePatternEvaluator::getLeftResults() {
  // Handles (VAR, _) / (VAR, _, _)
  std::vector<std::pair<std::string, std::string>> res = getDoubleSynResult();

  if (clause.isNegated) {
	res = negateDoubleSyn(res);
  }

  if (res.empty()) {
	return false;
  }

  insertDoubleColumnResult(res);
  return true;
}

std::vector<int> IfAndWhilePatternEvaluator::getAllStmts() {
  if (clause.clauseType == SimpleProgram::DesignAbstraction::PATTERN_IF) {
	return reader->getIfPatternStmtNum();
  } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PATTERN_WHILE) {
	return reader->getWhilePatternStmtNum();
  }

  return {};
}

std::vector<int> IfAndWhilePatternEvaluator::getAllStmts(const std::string &var) {
  if (clause.clauseType == SimpleProgram::DesignAbstraction::PATTERN_IF) {
	return reader->getIfPatternStmtNum(var);
  } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PATTERN_WHILE) {
	return reader->getWhilePatternStmtNum(var);
  }

  return {};
}

std::vector<std::string> IfAndWhilePatternEvaluator::getAllVariables() {
  if (clause.clauseType == SimpleProgram::DesignAbstraction::PATTERN_IF) {
	return reader->getIfPatternVariable();
  } else if (clause.clauseType == SimpleProgram::DesignAbstraction::PATTERN_WHILE) {
	return reader->getWhilePatternVariable();
  }

  return {};
}

std::vector<std::pair<std::string, std::string>> IfAndWhilePatternEvaluator::getDoubleSynResult() {
  std::vector<std::string> vars = getAllVariables();
  std::vector<int> stmtNums;
  std::vector<std::pair<std::string, std::string>> res;
  for (auto const &var : vars) {
	stmtNums = getAllStmts(var);
	for (int stmtNum : stmtNums) {
	  res.emplace_back(std::to_string(stmtNum), var);
	}
  }

  return res;
}

std::vector<std::pair<std::string, std::string>>
IfAndWhilePatternEvaluator::negateDoubleSyn(const std::vector<std::pair<std::string, std::string>> &selected) {
  std::vector<int> stmtNums = getAllStmts();
  std::vector<std::string> vars = reader->getAllVariables();
  std::vector<std::pair<std::string, std::string>> negatedRes;
  for (auto const &stmt : stmtNums) {
	for (auto const &v : vars) {
	  std::pair<std::string, std::string> pair(std::to_string(stmt), v);
	  if (std::find(selected.begin(), selected.end(), pair) == selected.end()) {
		negatedRes.push_back(pair);
	  }
	}
  }

  return negatedRes;
}
}