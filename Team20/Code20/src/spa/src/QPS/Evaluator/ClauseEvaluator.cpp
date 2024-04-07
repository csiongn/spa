#include <algorithm>
#include <unordered_set>

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
  switch (syn.entityType) {
	case SimpleProgram::DesignEntity::STMT:
	case SimpleProgram::DesignEntity::WILDCARD:
	  return reader->getAllStatementNum();
	case SimpleProgram::DesignEntity::READ:
	  return reader->getAllReadStmtNum();
	case SimpleProgram::DesignEntity::PRINT:
	  return reader->getAllPrintStmtNum();
	case SimpleProgram::DesignEntity::ASSIGN:
	  return reader->getAllAssignStmtNum();
	case SimpleProgram::DesignEntity::CALL:
	  return reader->getAllCallStmtNum();
	case SimpleProgram::DesignEntity::WHILE:
	  return reader->getAllWhileStmtNum();
	case SimpleProgram::DesignEntity::IF:
	  return reader->getAllIfStmtNum();
	case SimpleProgram::DesignEntity::CONSTANT:
	  return reader->getAllConstants();
	default:
	  // TODO: throw illegal argument, not allowed entity type for statement reference
	  return {};
  }
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