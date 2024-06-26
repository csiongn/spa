#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Models/PQL.h"
#include "PKB/facade/IPKBReader.h"
#include "Result.h"
#include "ResultStore.h"
#include "../Utils/EvaluatorUtils.h"

namespace QueryEvaluator {
class QueryEvaluator {
 public:
  std::shared_ptr<ResultStore> resultStore;
  std::shared_ptr<IPKBReader> reader;

  explicit QueryEvaluator(std::shared_ptr<IPKBReader> r);

  std::vector<std::string> evaluateQuery(PQL::Query &q);

 private:
  bool evaluateClause(const PQL::Clause &clause, bool createTable);

  void initialiseDeclaration(const PQL::Query &q);

  void addSynonymToStore(const PQL::Synonym &syn);

  std::vector<std::string> getStringResults(const PQL::Synonym &syn) const;

  std::vector<std::string> getStringResults(const PQL::Synonym &syn, int stmtNum) const;

  std::vector<int> getIntResults(const PQL::Synonym &syn) const;

  std::vector<std::pair<PQL::Clause, bool>> sortClauses(PQL::Query &q);

  static bool hasNoSynonym(PQL::Clause &cl);

  static bool isSynonym(const PQL::Synonym &arg);

  static std::vector<std::pair<std::unordered_set<std::string>,
							   std::vector<PQL::Clause>>> groupClauses(const std::vector<PQL::Clause> &cls,
																	   const std::unordered_set<int> &noSynonymClauses);

  static bool hasIntersection(const std::unordered_set<int> &s1, const std::unordered_set<int> &s2);

  static bool hasIntersection(const std::unordered_set<std::string> &s, const std::vector<std::string> &v);

  static void mergeKeyValuePair(std::unordered_map<std::unordered_set<std::string>,
												   std::unordered_set<int>,
												   EvaluatorUtils::SetHash> &map);

  void initialiseDoubleColumn(const PQL::Synonym &syn);

  static bool sortByVectorSize(const std::pair<std::unordered_set<std::string>, std::vector<PQL::Clause>> &a,
							   const std::pair<std::unordered_set<std::string>, std::vector<PQL::Clause>> &b);

  static bool compareClause(const PQL::Clause &c1, const PQL::Clause &c2);

  bool shouldInitialise(const PQL::Synonym &syn);

  std::unordered_map<SimpleProgram::DesignEntity, std::string> attrRefMap = {
	  {SimpleProgram::DesignEntity::CALL, "procName"},
	  {SimpleProgram::DesignEntity::READ, "varName"},
	  {SimpleProgram::DesignEntity::PRINT, "varName"},
  };

  std::string getAttrRefColName(const PQL::Synonym &syn);
};
}
