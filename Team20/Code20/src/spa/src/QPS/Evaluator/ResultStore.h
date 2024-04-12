#pragma once

#include <memory>
#include <string>
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
  }

  std::vector<std::string> retrieveSelect(const std::vector<PQL::Synonym> &selectSyns);

 private:
  void joinResults();

  std::vector<std::string> removeDuplicates(const std::vector<std::string> &v);

  std::string getSelectColName(const PQL::Synonym &selectSyn);
};
}