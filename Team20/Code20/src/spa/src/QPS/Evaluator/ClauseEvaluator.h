#pragma once

#include <memory>
#include <string>
#include <utility>
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
  bool createTable;

 public:
  ClauseEvaluator(std::shared_ptr<IPKBReader> r,
				  const PQL::Clause &cl,
				  std::shared_ptr<ResultStore> _resultStore,
				  bool _createTable)
	  : reader(r), clause(cl), createTable(_createTable) {
	resultStore = _resultStore;
  };

  virtual bool evaluate() = 0;

  static std::vector<int> getIntersection(std::vector<int> &v1, std::vector<int> &v2);

  std::vector<int> getAllIntResults(const PQL::Synonym &syn);

  std::vector<int> negateIntResults(const PQL::Synonym &syn, const std::vector<int> &selected);

  void insertDoubleColumnResult(const std::vector<std::pair<std::string, std::string>> &result);
};
}
