#include <utility>
#include <unordered_map>

#include "PatternEvaluator.h"

namespace QueryEvaluator {

void PatternEvaluator::insertDoubleColumnResult(const std::vector<std::pair<std::string, std::string>> &result) {
  PQL::Synonym patternSyn = clause.arguments[0];
  PQL::Synonym lArg = clause.arguments[1];
  std::vector<std::vector<std::string>> table = {{}, {}};
  std::vector<std::string> colNames = {patternSyn.identity, lArg.identity};
  std::unordered_map<std::string, size_t> colNameToIndex;
  for (size_t i = 0; i < colNames.size(); i++) {
	colNameToIndex[colNames[i]] = i;
  }
  Result newResult{table, colNames, colNameToIndex};

  for (auto const &pair : result) {
	newResult.addRow({std::get<0>(pair), std::get<1>(pair)});
  }

  resultStore->insertResult(std::make_shared<Result>(newResult));
}
}