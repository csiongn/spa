#pragma once

#include <memory>

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
};
}