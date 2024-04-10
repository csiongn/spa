#include "IfAndWhilePatternEvaluator.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

namespace QueryEvaluator {
bool IfAndWhilePatternEvaluator::evaluate() {
  std::unordered_map<SimpleProgram::DesignEntity, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignEntity::VARIABLE, [this] { return getForwardRelationship(); }},
	  {SimpleProgram::DesignEntity::WILDCARD, [this] { return hasAtLeastOneRelationship(); }},
	  {SimpleProgram::DesignEntity::IDENT, [this] { return hasRelationship(); }},
  };

  PQL::Synonym lArg = clause.arguments[1];
  if (funcMap.find(lArg.entityType) == funcMap.end()) {
	// unhandled cases
	return false;
  }

  return funcMap[lArg.entityType]();
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
  std::unordered_map<SimpleProgram::DesignEntity, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignEntity::IF, [this] { return reader->hasIfPattern(); }},
	  {SimpleProgram::DesignEntity::WHILE, [this] { return reader->hasWhilePattern(); }},
  };

  PQL::Synonym patternSyn = clause.arguments[0];
  bool isNotEmpty = false;
  if (funcMap.find(patternSyn.entityType) != funcMap.end()) {
	isNotEmpty = funcMap[patternSyn.entityType]();
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
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<int>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::PATTERN_IF, [this] { return reader->getIfPatternStmtNum(); }},
	  {SimpleProgram::DesignAbstraction::PATTERN_WHILE, [this] { return reader->getWhilePatternStmtNum(); }},
  };

  if (funcMap.find(clause.clauseType) == funcMap.end()) {
	return {};
  }

  return funcMap[clause.clauseType]();
}

std::vector<int> IfAndWhilePatternEvaluator::getAllStmts(const std::string &var) {
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<int>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::PATTERN_IF, [this, var] { return reader->getIfPatternStmtNum(var); }},
	  {SimpleProgram::DesignAbstraction::PATTERN_WHILE, [this, var] { return reader->getWhilePatternStmtNum(var); }},
  };

  if (funcMap.find(clause.clauseType) == funcMap.end()) {
	return {};
  }

  return funcMap[clause.clauseType]();
}

std::vector<std::string> IfAndWhilePatternEvaluator::getAllVariables() {
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<std::string>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::PATTERN_IF, [this] { return reader->getIfPatternVariable(); }},
	  {SimpleProgram::DesignAbstraction::PATTERN_WHILE, [this] { return reader->getWhilePatternVariable(); }},
  };

  if (funcMap.find(clause.clauseType) == funcMap.end()) {
	return {};
  }

  return funcMap[clause.clauseType]();
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