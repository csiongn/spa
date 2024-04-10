#include <algorithm>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#include "ClauseEvaluator.h"

namespace QueryEvaluator {

std::vector<int> ClauseEvaluator::getIntersection(std::vector<int> &v1, std::vector<int> &v2) {
  std::vector<int> intersection;
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  std::set_intersection(v1.begin(), v1.end(),
						v2.begin(), v2.end(), std::back_inserter(intersection));
  return intersection;
}

std::vector<int> ClauseEvaluator::getAllIntResults(const PQL::Synonym &syn) {
  std::unordered_map<SimpleProgram::DesignEntity, std::function<std::vector<int>()>> funcMap = {
	  {SimpleProgram::DesignEntity::STMT, [this] { return reader->getAllStatementNum(); }},
	  {SimpleProgram::DesignEntity::WILDCARD, [this] { return reader->getAllStatementNum(); }},
	  {SimpleProgram::DesignEntity::READ, [this] { return reader->getAllReadStmtNum(); }},
	  {SimpleProgram::DesignEntity::PRINT, [this] { return reader->getAllPrintStmtNum(); }},
	  {SimpleProgram::DesignEntity::ASSIGN, [this] { return reader->getAllAssignStmtNum(); }},
	  {SimpleProgram::DesignEntity::CALL, [this] { return reader->getAllCallStmtNum(); }},
	  {SimpleProgram::DesignEntity::WHILE, [this] { return reader->getAllWhileStmtNum(); }},
	  {SimpleProgram::DesignEntity::IF, [this] { return reader->getAllIfStmtNum(); }},
	  {SimpleProgram::DesignEntity::CONSTANT, [this] { return reader->getAllConstants(); }}
  };

  if (funcMap.find(syn.entityType) == funcMap.end()) {
	return {};
  }

  return funcMap[syn.entityType]();
}

std::vector<int> ClauseEvaluator::negateIntResults(const PQL::Synonym &syn, const std::vector<int> &selected) {
  std::vector<int> intResults = getAllIntResults(syn);
  std::vector<int> negatedResults;
  std::unordered_set<int> selectedSet(selected.begin(), selected.end());

  for (auto const &val : intResults) {
	if (selectedSet.find(val) == selectedSet.end()) {
	  negatedResults.push_back(val);
	}
  }

  return negatedResults;
}
}