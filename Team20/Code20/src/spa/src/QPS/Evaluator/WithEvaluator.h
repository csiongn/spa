#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
class WithEvaluator : private ClauseEvaluator {
 public:
  WithEvaluator(std::shared_ptr<IPKBReader> r,
				const PQL::Clause &cl,
				std::shared_ptr<ResultStore> resultStore,
				bool createTable)
	  : ClauseEvaluator(r, cl, resultStore, createTable) {
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

  static bool hasIntResults(const PQL::Synonym &syn);

  std::pair<std::vector<std::string>, std::vector<std::string>> retrieveIntResults(const PQL::Synonym &syn,
																				   const std::vector<std::string> &values);

  template<typename T>
  void checkAndInsertResult(const PQL::Synonym &lArg,
							const std::vector<T> &lValues,
							const PQL::Synonym &rArg,
							const std::vector<T> &rValues);

  std::unordered_map<SimpleProgram::DesignEntity, std::string> attrRefMap = {
	  {SimpleProgram::DesignEntity::CALL, "procName"},
	  {SimpleProgram::DesignEntity::READ, "varName"},
	  {SimpleProgram::DesignEntity::PRINT, "varName"},
  };

  std::string getAttrRefColName(const PQL::Synonym &syn);

  static std::vector<std::string> intToStringVector(const std::vector<int>& v);
};
}