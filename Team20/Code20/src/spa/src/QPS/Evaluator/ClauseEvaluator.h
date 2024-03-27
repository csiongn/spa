#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "PKB/facade/IPKBReader.h"
#include "Result.h"
#include "ResultStore.h"

namespace QueryEvaluator {
class ClauseEvaluator {
 protected:
  std::shared_ptr<IPKBReader> reader;
  PQL::Clause clause;
  std::shared_ptr<ResultStore> resultStore;

 public:
  ClauseEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, std::shared_ptr<ResultStore> _resultStore)
	  : reader(r), clause(cl) {
	resultStore = _resultStore;
  };

  virtual bool evaluate() = 0;

  static std::vector<int> getIntersection(std::vector<int> &v1, std::vector<int> &v2);

  std::vector<int> getStmtNums(const PQL::Synonym &syn);
};
}
