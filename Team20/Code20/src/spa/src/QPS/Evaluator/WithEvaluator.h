#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
class WithEvaluator : private ClauseEvaluator {
 public:
  WithEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, std::shared_ptr<ResultStore> resultStore)
	  : ClauseEvaluator(r, cl, resultStore) {
  };

  bool evaluate() override;

 private:
  bool isAlwaysFalse();

  bool isAlwaysTrue();

  bool handleSingleAttrRef();

  bool handleDoubleAttrRef();

  static bool hasIntegerAttrRef(const PQL::Synonym &syn);

  static bool hasNameAttrRef(const PQL::Synonym &syn);

  static bool hasMultiAttrRef(const PQL::Synonym &syn);

  static bool canCompare(const PQL::Synonym &lArg, const PQL::Synonym &rArg);

  std::vector<std::string> getIdentValues(const PQL::Synonym &syn);

  template<typename T>
  bool createDoubleColumnResult(const PQL::Synonym &lArg,
								const PQL::Synonym &rArg,
								std::vector<T> &lValues,
								std::vector<T> &rValues);

  std::vector<std::string> getStringIntersection(std::vector<std::string> &v1, std::vector<std::string> &v2);

  std::vector<std::string> negateStringResults(const PQL::Synonym &syn, const std::vector<std::string> &selected);

  template<typename T>
  std::vector<T> negateResults(const PQL::Synonym &syn, const std::vector<T> &selected);

  static bool isIntResultsOnly(const PQL::Synonym &syn);

  std::vector<int> retrieveIntResults(const PQL::Synonym &syn, const std::vector<std::string> &values);

  template<typename T>
  void checkAndInsertResult(const PQL::Synonym &syn, const std::vector<T> &values);
};
}