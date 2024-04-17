#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "Result.h"

namespace QueryEvaluator {
class ResultStore {
 public:
  std::vector<std::shared_ptr<Result>> results;

  ResultStore() = default;

  void insertResult(std::shared_ptr<Result> r);

  template<typename T>
  void createColumn(const PQL::Synonym &syn, const std::vector<T> &values) {
	std::shared_ptr<Result> res = std::make_shared<Result>(syn.identity, values);
	results.push_back(res);
	addedIdents.insert(syn.identity);
  }

  std::vector<std::string> retrieveSelect(const std::vector<PQL::Synonym> &selectSyns);

  bool containsColumn(const std::string &ident);

 private:
  std::unordered_set<std::string> addedIdents;

  void joinResults();

  std::vector<std::string> removeDuplicates(const std::vector<std::string> &v);

  std::string getSelectColName(const PQL::Synonym &selectSyn);

  std::vector<std::string> getSelectedColsAtRow(std::shared_ptr<Result> res,
												const std::vector<size_t> &selectSynIndexes,
												size_t rowIndex);
};
}