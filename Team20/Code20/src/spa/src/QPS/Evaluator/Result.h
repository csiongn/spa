#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Models/PQL.h"
#include "../Utils/EvaluatorUtils.h"

namespace QueryEvaluator {
class Result {
 public:
  // table.size() = how many columns, need to match colNames.size()
  // table[0].size() = how many rows
  // table[i][j] = element at ith column, jth row
  std::vector<std::vector<std::string>> table;
  std::vector<std::string> colNames;
  std::unordered_map<std::string, size_t> colNameToIndex;
  bool empty;

  // add by columns
  Result(const std::string &colName, const std::vector<std::string> &rows);

  Result(const std::string &colName, const std::vector<int> &rows);

  Result(const std::vector<std::vector<std::string>> &r, std::vector<std::string> &cNames,
		 std::unordered_map<std::string, size_t> colNameToIndex);

  Result() = default;

  std::vector<std::string> getCommonColumns(const Result &r);

  std::shared_ptr<Result> hashJoin(Result &result, std::vector<std::string> commonCols);

  std::shared_ptr<Result> crossJoin(const Result &r);

  void addRow(const std::vector<std::string> &row);

 private:
  std::vector<std::string> getColumn(const std::string &colName);

  std::vector<std::string> getRow(size_t columnIndex) const;

  static std::vector<std::string>
  joinRows(const std::vector<std::string> &row1, const std::unordered_map<std::string, size_t> &colNamesToIndex1,
		   const std::vector<std::string> &row2, const std::unordered_map<std::string, size_t> &colNamesToIndex2,
		   const std::unordered_map<std::string, size_t> &joinedColNamesToIndex);

  static std::unordered_map<std::string, size_t>
  joinColNamesToIndex(const std::unordered_map<std::string, size_t> &colNamesToIndex1,
					  const std::unordered_map<std::string, size_t> &colNamesToIndex2);

  static std::vector<std::string> getColumnNames(const std::unordered_map<std::string, size_t> &colNameToIndexMap);

  std::unordered_map<std::vector<std::string>,
					 std::unordered_set<std::vector<std::string>, EvaluatorUtils::vectorHash>,
					 EvaluatorUtils::vectorHash> getCommonColMap(std::vector<std::string> commonCols);
};
}