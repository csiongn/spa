#pragma once

#include <memory>
#include <utility>
#include <string>
#include <vector>

#include "PatternEvaluator.h"

namespace QueryEvaluator {
class IfAndWhilePatternEvaluator : private PatternEvaluator {
 public:
  IfAndWhilePatternEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl,
							 std::shared_ptr<ResultStore> resultStore)
	  : PatternEvaluator(r, cl, resultStore) {};

  bool evaluate() override;

 private:
  bool hasRelationship() override;

  bool hasAtLeastOneRelationship() override;

  bool getForwardRelationship() override;

  bool getLeftResults() override;

  bool getSynonymWildcard() override;

  std::vector<int> getAllStmts();

  std::vector<int> getAllStmts(const std::string &var);

  std::vector<std::string> getAllVariables();

  std::vector<std::pair<std::string, std::string>> getDoubleSynResult() override;

  std::vector<std::pair<std::string, std::string>> negateDoubleSyn(const std::vector<std::pair<std::string,
																							   std::string>> &selected) override;
};
}