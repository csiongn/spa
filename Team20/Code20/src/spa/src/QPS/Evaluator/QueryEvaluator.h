#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "PKB/facade/IPKBReader.h"
#include "Result.h"
#include "ResultStore.h"

namespace QueryEvaluator {
class QueryEvaluator {
 public:
  std::shared_ptr<ResultStore> resultStore;
  std::shared_ptr<IPKBReader> reader;

  explicit QueryEvaluator(std::shared_ptr<IPKBReader> r);

  std::vector<std::string> evaluateQuery(const PQL::Query &q);

 private:
  bool evaluateClause(const PQL::Clause &clause);

  void initialiseDeclaration(const PQL::Query &q);

  void addSynonymToStore(const PQL::Synonym &syn);

  std::vector<std::string> getStringResults(const PQL::Synonym &syn) const;

  std::vector<int> getIntResults(const PQL::Synonym &syn) const;
};
}
