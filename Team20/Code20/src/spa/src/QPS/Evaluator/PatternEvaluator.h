#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
class PatternEvaluator : protected ClauseEvaluator {
 public:
  PatternEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, std::shared_ptr<ResultStore> resultStore)
	  : ClauseEvaluator(r, cl, resultStore) {};

  bool evaluate() override = 0;

 private:
  virtual bool hasRelationship() = 0;

  virtual bool hasAtLeastOneRelationship() = 0;

  virtual bool getForwardRelationship() = 0;

  virtual bool getLeftResults() = 0;

  virtual bool getSynonymWildcard() = 0;
};
}