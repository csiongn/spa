#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "PatternEvaluator.h"

namespace QueryEvaluator {
class AssignPatternEvaluator : private PatternEvaluator {
 public:
  AssignPatternEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl,
						 std::shared_ptr<ResultStore> resultStore)
	  : PatternEvaluator(r, cl, resultStore) {};

  bool evaluate() override;

 private:
  bool hasRelationship() override;

  bool hasRelationship(const std::string &ident, const std::string &expr);

  bool hasAtLeastOneRelationship() override;

  bool getForwardRelationship() override;

  bool getLeftResults() override;

  bool getSynonymWildcard() override;

  std::vector<std::shared_ptr<ExprNode>> getAllPartialNodes(const std::shared_ptr<ExprNode> &exprNode);

  std::vector<std::string> getAllAssignVar(const std::vector<std::shared_ptr<ExprNode>> &nodes);

  std::vector<int>
  getAssignSynResults(const std::vector<std::string> &vars, const std::vector<std::shared_ptr<ExprNode>> &exprNodes);

  std::vector<std::pair<std::string, std::string>> getDoubleSynResult() override;

  std::vector<std::pair<std::string, std::string>> negateDoubleSyn(const std::vector<std::pair<std::string,
																							   std::string>> &selected) override;
};
}