#pragma once

#include <memory>

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
class IfAndWhilePatternEvaluator : private ClauseEvaluator {
 public:
  IfAndWhilePatternEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl,
							 std::shared_ptr<ResultStore> resultStore)
	  : ClauseEvaluator(r, cl, resultStore) {};

  bool evaluate() override;

 private:
  bool isAlwaysFalse() override;

  bool hasRelationship() override;

  bool hasAtLeastOneRelationship() override;

  bool getForwardRelationship() override;

  bool getReversedRelationship() override;

  bool getLeftResults() override;

  bool getSynonymWildcard() override;

  bool getRightResults() override;

  bool getWildcardSynonym() override;

  bool getDoubleSynonym() override;
};
}