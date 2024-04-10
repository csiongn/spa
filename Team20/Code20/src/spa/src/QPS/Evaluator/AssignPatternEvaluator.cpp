#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "AssignPatternEvaluator.h"
#include "../Utils/ParseUtils.h"

namespace QueryEvaluator {

bool AssignPatternEvaluator::evaluate() {
  PQL::Synonym lArg = clause.arguments[1];
  PQL::Synonym rArg = clause.arguments[2];

  if (lArg.entityType == rArg.entityType && lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	// handles (_, _)
	return hasAtLeastOneRelationship();
  }

  if (lArg.entityType == SimpleProgram::DesignEntity::IDENT) {
	if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	  // (IDENT, _)
	  return hasAtLeastOneRelationship();
	}

	// Handles (IDENT, EXPR), (IDENT, PARTIAL_EXPR)
	return hasRelationship();
  }

  std::unordered_map<SimpleProgram::DesignEntity, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignEntity::VARIABLE, [this] { return getForwardRelationship(); }},
	  {SimpleProgram::DesignEntity::WILDCARD, [this] { return hasAtLeastOneRelationship(); }},
  };

  if (funcMap.find(lArg.entityType) == funcMap.end()) {
	// unhandled cases
	return false;
  }

  return funcMap[lArg.entityType]();
}

bool AssignPatternEvaluator::hasRelationship() {
  // handles (IDENT, EXPR) & (IDENT, PARTIAL_EXPR)
  PQL::Synonym assignSyn = clause.arguments[0];
  PQL::Synonym lArg = clause.arguments[1];
  PQL::Synonym rArg = clause.arguments[2];

  bool isNotEmpty = false;
  if (rArg.entityType == SimpleProgram::DesignEntity::EXPR) {
	// (IDENT, EXPR)
	isNotEmpty = hasRelationship(lArg.identity, rArg.identity);
	if (clause.isNegated) {
	  isNotEmpty = !isNotEmpty;
	}
  } else {
	// (IDENT, PARTIAL_EXPR)
	std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(rArg.identity);
	std::shared_ptr<ExprNode> exprNode = ParseUtils::getExprNode(std::get<0>(tup));

	// get all partial expr nodes
	std::vector<std::shared_ptr<ExprNode>> partialExprNodes = getAllPartialNodes(exprNode);

	// use IDENT and partial expr nodes to get the related assign stmt nums
	std::vector<int> assignSynResults = getAssignSynResults({lArg.identity}, partialExprNodes);

	if (clause.isNegated) {
	  assignSynResults = negateIntResults(lArg, assignSynResults);
	}

	isNotEmpty = !assignSynResults.empty();
	if (isNotEmpty) {
	  resultStore->createColumn(assignSyn, assignSynResults);
	}
  }

  return isNotEmpty;
}

bool AssignPatternEvaluator::hasRelationship(const std::string &ident, const std::string &expr) {
  std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(expr);
  std::shared_ptr<ExprNode> exprNode = ParseUtils::getExprNode(std::get<0>(tup));
  std::vector<int> assignStmtNums = reader->getAssignPatternStmtNum(ident, exprNode->getHashValue());

  if (assignStmtNums.empty()) {
	return false;
  }

  // add ASSIGN_SYN to result store
  resultStore->createColumn(clause.arguments[0], assignStmtNums);
  return true;
}

bool AssignPatternEvaluator::hasAtLeastOneRelationship() {
  // handles (_, _) and (IDENT, _), (_, EXPR), (_, PARTIAL_EXPR)
  PQL::Synonym assignSyn = clause.arguments[0];
  PQL::Synonym lArg = clause.arguments[1];
  PQL::Synonym rArg = clause.arguments[2];

  bool isNotEmpty;
  std::vector<int> assignStmtNums;
  if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(rArg.identity);
	std::shared_ptr<ExprNode> exprNode = ParseUtils::getExprNode(std::get<0>(tup));
	if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	  // not adding ASSIGN_SYN to result store, result same as all assign statements
	  // already added during initialise SYN in QueryEvaluator
	  isNotEmpty = reader->hasAssignPattern();
	  if (clause.isNegated) {
		isNotEmpty = !isNotEmpty;
	  }
	  return isNotEmpty;
	}

	if (rArg.entityType == SimpleProgram::DesignEntity::EXPR) {
	  // (_, EXPR)
	  assignStmtNums = reader->getAssignPatternRHSStmtNum(exprNode->getHashValue());
	} else {
	  // (_, PARTIAL_EXPR)
	  std::vector<std::shared_ptr<ExprNode>> partialExprNodes = getAllPartialNodes(exprNode);

	  std::unordered_set<int> assignStmtNumsSet;
	  for (auto const &partialExprNode : partialExprNodes) {
		std::vector<int> stmtNums = reader->getAssignPatternRHSStmtNum(partialExprNode->getHashValue());
		assignStmtNumsSet.insert(stmtNums.begin(), stmtNums.end());
	  }

	  assignStmtNums = {assignStmtNumsSet.begin(), assignStmtNumsSet.end()};
	}
  } else {
	// (IDENT, _)
	assignStmtNums = reader->getAssignPatternLHSStmtNum(lArg.identity);
  }

  if (clause.isNegated) {
	assignStmtNums = negateIntResults(assignSyn, assignStmtNums);
  }

  isNotEmpty = !assignStmtNums.empty();
  if (isNotEmpty) {
	resultStore->createColumn(assignSyn, assignStmtNums);
  }
  return isNotEmpty;
}

bool AssignPatternEvaluator::getForwardRelationship() {
  // Handles (VAR, EXPR), (VAR, PARTIAL_EXPR), (VAR, _)
  PQL::Synonym assignSyn = clause.arguments[0];
  PQL::Synonym lArg = clause.arguments[1];
  PQL::Synonym rArg = clause.arguments[2];

  std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(rArg.identity);
  std::shared_ptr<ExprNode> exprNode = ParseUtils::getExprNode(std::get<0>(tup));;
  if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	// (VAR, _)
	return getSynonymWildcard();
  } else {
	// (VAR, EXPR), (VAR, PARTIAL_EXPR)
	return getLeftResults();
  }
}

bool AssignPatternEvaluator::getLeftResults() {
  // (VAR, EXPR), (VAR, PARTIAL_EXPR)

  std::vector<std::pair<std::string, std::string>> result = getDoubleSynResult();
  if (clause.isNegated) {
	result = negateDoubleSyn(result);
  }

  bool isNotEmpty = !result.empty();
  if (isNotEmpty) {
	insertDoubleColumnResult(result);
  }
  return isNotEmpty;
}

bool AssignPatternEvaluator::getSynonymWildcard() {
  // Handles (VAR, _)
  PQL::Synonym assignSyn = clause.arguments[0];
  PQL::Synonym lArg = clause.arguments[1];

  std::vector<std::string> lResults = reader->getAssignPatternLHS();
  std::vector<std::pair<std::string, std::string>> result;
  for (auto const &var : lResults) {
	std::vector<int> stmtNums = reader->getAssignPatternLHSStmtNum(var);
	for (int stmtNum : stmtNums) {
	  result.emplace_back(std::to_string(stmtNum), var);
	}
  }

  if (clause.isNegated) {
	result = negateDoubleSyn(result);
  }

  bool isNotEmpty = !result.empty();
  if (isNotEmpty) {
	insertDoubleColumnResult(result);
  }

  return isNotEmpty;
}

std::vector<std::shared_ptr<ExprNode>> AssignPatternEvaluator::getAllPartialNodes(const std::shared_ptr<ExprNode> &exprNode) {
  // get all RHS
  std::vector<size_t> allHashes = reader->getAssignPatternRHS();

  // use all RHS to get all nodes
  std::vector<std::shared_ptr<ExprNode>> allNodes;
  for (auto const &hash : allHashes) {
	std::vector<std::shared_ptr<ExprNode>> nodes = reader->getAssignPatternRHSExprNodePtr(hash);
	allNodes.reserve(allNodes.size() + nodes.size());
	allNodes.insert(allNodes.end(), nodes.begin(), nodes.end());
  }

  // filter nodes to only those that contains the partial expr
  std::vector<std::shared_ptr<ExprNode>> filteredNodes;
  filteredNodes.reserve(allNodes.size());
  for (auto const &node : allNodes) {
	if (node->containsSubtree(*(exprNode))) {
	  filteredNodes.push_back(node);
	}
  }

  return filteredNodes;
}

std::vector<std::string> AssignPatternEvaluator::getAllAssignVar(const std::vector<std::shared_ptr<ExprNode>> &nodes) {
  std::unordered_set<std::string> varSet;
  for (auto const &node : nodes) {
	std::vector<std::string> leftVars = reader->getAssignPatternLHS(node->getHashValue());
	varSet.reserve(varSet.size() + leftVars.size());
	varSet.insert(leftVars.begin(), leftVars.end());
  }

  std::vector<std::string> vars;
  vars.reserve(varSet.size());
  vars.insert(vars.end(), varSet.begin(), varSet.end());
  return vars;
}

std::vector<int> AssignPatternEvaluator::getAssignSynResults(const std::vector<std::string> &vars,
															 const std::vector<std::shared_ptr<ExprNode>> &exprNodes) {
  std::unordered_set<int> assignSynResultsSet;
  for (auto const &var : vars) {
	for (auto const &exprNode : exprNodes) {
	  std::vector<int> assignStmtNums = reader->getAssignPatternStmtNum(var, exprNode->getHashValue());
	  assignSynResultsSet.reserve(assignSynResultsSet.size() + assignStmtNums.size());
	  assignSynResultsSet.insert(assignStmtNums.begin(), assignStmtNums.end());
	}
  }

  // add ASSIGN_SYN to result store
  std::vector<int> assignSynResults;
  assignSynResults.reserve(assignSynResultsSet.size());
  assignSynResults.insert(assignSynResults.end(), assignSynResultsSet.begin(), assignSynResultsSet.end());

  return assignSynResults;
}

std::vector<std::pair<std::string, std::string>> AssignPatternEvaluator::getDoubleSynResult() {
  PQL::Synonym rArg = clause.arguments[2];
  std::tuple<std::string, bool, bool> tup = ParseUtils::parsePattern(rArg.identity);
  std::shared_ptr<ExprNode> exprNode = ParseUtils::getExprNode(std::get<0>(tup));
  std::vector<std::pair<std::string, std::string>> result;
  std::vector<std::string> vars;
  std::vector<int> assignSynResults;

  if (std::get<1>(tup) && std::get<2>(tup)) {
	// (VAR, PARTIAL_EXPR)
	std::unordered_set<int> assignSynResultSet;
	std::vector<std::shared_ptr<ExprNode>> partialExprNodes = getAllPartialNodes(exprNode);
	vars = getAllAssignVar(partialExprNodes);
	for (auto const &var : vars) {
	  for (auto const &node : partialExprNodes) {
		std::vector<int> stmtNums = reader->getAssignPatternStmtNum(var, node->getHashValue());
		assignSynResultSet.insert(stmtNums.begin(), stmtNums.end());
	  }
	}
	assignSynResults = std::vector<int>(assignSynResultSet.begin(), assignSynResultSet.end());
  } else {
	// (VAR, EXPR)
	vars = reader->getAssignPatternLHS(exprNode->getHashValue());
	assignSynResults = getAssignSynResults(vars, {exprNode});
  }

  for (auto const &var : vars) {
	for (auto const &stmtNum : assignSynResults) {
	  result.emplace_back(std::to_string(stmtNum), var);
	}
  }

  return result;
}

std::vector<std::pair<std::string, std::string>>
AssignPatternEvaluator::negateDoubleSyn(const std::vector<std::pair<std::string, std::string>> &selected) {
  std::vector<std::string> vars = reader->getAllVariables();
  std::vector<int> stmtNums = reader->getAllAssignStmtNum();
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