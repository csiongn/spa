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
								const std::vector<T> &rValues);
};
}