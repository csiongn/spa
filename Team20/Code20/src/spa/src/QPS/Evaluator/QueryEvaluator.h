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

  std::vector<int> getIntResults(const PQL::Synonym &syn) const;

  std::vector<std::pair<PQL::Clause, bool>> sortClauses(PQL::Query &q);

  static bool hasNoSynonym(PQL::Clause &cl);

  static bool isSynonym(const PQL::Synonym &arg);

  static std::vector<std::pair<std::unordered_set<std::string>,
							   std::vector<PQL::Clause>>> groupClauses(const std::vector<PQL::Clause> &cls,
																	   const std::unordered_set<int> &noSynonymClauses);

  static bool hasIntersection(const std::unordered_set<int> &s1, const std::unordered_set<int> &s2);

  static bool hasIntersection(const std::unordered_set<std::string> &s, const std::vector<std::string> &v);

  // Custom hash function to hash a set of string
  struct SetHash {
	size_t operator()(const std::unordered_set<std::string> &set) const {
	  size_t seed = 0;
	  for (const auto &element : set) {
		seed ^= std::hash<std::string>()(element) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	  }
	  return seed;
	}
  };

  static void mergeKeyValuePair(std::unordered_map<std::unordered_set<std::string>,
												   std::unordered_set<int>,
												   SetHash> &map);
};
}
