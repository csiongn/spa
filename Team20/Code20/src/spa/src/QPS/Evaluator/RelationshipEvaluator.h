#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
class RelationshipEvaluator : protected ClauseEvaluator {
 public:
  RelationshipEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, std::shared_ptr<ResultStore> resultStore)
	  : ClauseEvaluator(r, cl, resultStore) {};

  bool evaluate() override = 0;

 private:
  virtual bool isAlwaysFalse() = 0;

  virtual bool hasRelationship() = 0;

  virtual bool hasAtLeastOneRelationship() = 0;

  virtual bool getForwardRelationship() = 0;

  virtual bool getReversedRelationship() = 0;

  virtual bool getLeftResults() = 0;

  virtual bool getSynonymWildcard() = 0;

  virtual bool getRightResults() = 0;

  virtual bool getWildcardSynonym() = 0;

  virtual bool getDoubleSynonym() = 0;
};
}