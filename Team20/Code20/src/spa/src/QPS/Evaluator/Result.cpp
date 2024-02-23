#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Result.h"

namespace QueryEvaluator {
    // inserting one column of string
    Result::Result(const std::string &colName, const std::vector<std::string> &rows) {
        if (rows.empty()) {
            return;
        }

        // table with one empty column
        table = {{}};
        colNames.push_back(colName);
        colNameToIndex[colName] = 0;

        for (auto &v: rows) {
            // add to first column
            table[0].push_back({v});
        }

        empty = false;
    }

    // inserting one column of int, convert to string first
    Result::Result(const std::string &colName, const std::vector<int> &rows) {
        if (rows.empty()) {
            return;
        }

        // table with one empty column
        table = {{}};
        colNames.push_back(colName);
        colNameToIndex[colName] = 0;

        std::vector<std::string> strRows(rows.size());
        std::transform(rows.begin(), rows.end(), strRows.begin(), [](int intVal) -> std::string {
            return std::to_string(intVal);
        });

        for (auto &v: strRows) {
            // add to first column
            table[0].push_back({v});
        }

        empty = false;
    }

    Result::Result(const std::vector<std::vector<std::string>> &t,
                   std::vector<std::string> &cNames, std::unordered_map<std::string, size_t> colNameIndex)
            : table(t), colNames(cNames), colNameToIndex(std::move(colNameIndex)), empty(true) {}

    std::vector<std::string> Result::getCommonColumns(const QueryEvaluator::Result &r) {
        std::vector<std::string> commonCols;
        std::set_intersection(colNames.begin(), colNames.end(),
                              r.colNames.begin(), r.colNames.end(), std::back_inserter(commonCols));
        return commonCols;
    }

    std::shared_ptr<Result> Result::crossJoin(const QueryEvaluator::Result &result) {
        std::vector<std::vector<std::string>> newTable;
        std::vector<std::string> newColNames;
        std::unordered_map<std::string, size_t> newColNameToIndex;

        // initialize newTable, newColNames & newColNameToIndex
        newColNameToIndex = joinColNamesToIndex(colNameToIndex, result.colNameToIndex);
        newColNames = getColumnNames(newColNameToIndex);
        newTable.reserve(newColNames.size());
        for (auto i = 0; i < newColNames.size(); ++i) {
            std::vector<std::string> col;
            size_t maxRows = std::max(table[0].size(), result.table[0].size());
            col.reserve(maxRows);
            newTable.push_back(col);
        }
        Result newResult{newTable, newColNames, newColNameToIndex};

        for (auto i = 0; i < table[0].size(); ++i) {
            std::vector<std::string> outerRow = getRow(i);
            for (auto j = 0; j < result.table[0].size(); ++j) {
                std::vector<std::string> innerRow = result.getRow(j);
                std::vector<std::string> newRow = joinRows(outerRow, colNameToIndex, innerRow, result.colNameToIndex,
                                                           newColNameToIndex);
                newResult.addRow(newRow);
            }
        }
        return std::make_shared<Result>(newResult);
    }

    std::shared_ptr<Result>
    Result::hashJoin(QueryEvaluator::Result &result, std::vector<std::string> commonCols) {
        std::vector<std::vector<std::string>> newTable;
        std::vector<std::string> newColNames;
        std::unordered_map<std::string, size_t> newColNameToIndex;

        // initialize newTable, newColNames & newColNameToIndex
        newColNameToIndex = joinColNamesToIndex(colNameToIndex, result.colNameToIndex);
        newColNames = getColumnNames(newColNameToIndex);
        newTable.reserve(newColNames.size());
        for (auto i = 0; i < newColNames.size(); ++i) {
            std::vector<std::string> col;
            size_t maxRows = std::max(table[0].size(), result.table[0].size());
            col.reserve(maxRows);
            newTable.push_back(col);
        }
        Result newResult{newTable, newColNames, newColNameToIndex};

        std::unordered_map<size_t, std::tuple<size_t, size_t>> newRowToOldRows;
        // initialize newTable using the first common column
        for (auto i = 0; i < getColumn(commonCols[0]).size(); ++i) {
            for (auto j = 0; j < result.getColumn(commonCols[0]).size(); ++j) {
                std::vector<std::string> outerRow = getRow(i);
                std::vector<std::string> innerRow = result.getRow(j);
                if (outerRow[colNameToIndex.at(commonCols[0])] != innerRow[result.colNameToIndex.at(commonCols[0])]) {
                    // different value at the commonColumn, do not add the row.
                    continue;
                }

                std::vector<std::string> newRow = joinRows(outerRow, colNameToIndex, innerRow, result.colNameToIndex,
                                                           newColNameToIndex);
                newRowToOldRows[newResult.table.size()] = {i, j};
                newResult.addRow(newRow);
            }
        }

        std::unordered_set<size_t> rowsToRemove;
        for (auto &colName: commonCols) {
            if (colName == commonCols[0]) {
                // skip first common column because it has already been added.
                continue;
            }

            size_t colIndex1 = colNameToIndex[colName];
            size_t colIndex2 = result.colNameToIndex[colName];
            for (size_t row = 0; row < newResult.table[0].size(); ++row) {
                size_t rowIndex1 = std::get<0>(newRowToOldRows[row]);
                size_t rowIndex2 = std::get<1>(newRowToOldRows[row]);
                if (newTable[colIndex1][rowIndex1] != newTable[colIndex2][rowIndex2]) {
                    rowsToRemove.insert(row);
                }
            }
        }

        // remove rows
        if (rowsToRemove.empty()) {
            return std::make_shared<Result>(newResult);
        }

        std::vector<std::vector<std::string>> newFilteredTable;
        Result newFilteredResult{newFilteredTable, newColNames, newColNameToIndex};
        for (auto row = 0; row < newTable[0].size(); ++row) {
            if (rowsToRemove.count(row) < 1) {
                // this row does not need to be removed
                newFilteredResult.addRow(newResult.getRow(row));
            }
        }

        return std::make_shared<Result>(newFilteredResult);
    }

    std::vector<std::string> Result::getRow(size_t rowIndex) const {
        std::vector<std::string> row;
        row.reserve(table.size());
        for (const auto & col : table) {
            row.push_back(col[rowIndex]);
        }
        return row;
    }

    std::vector<std::string> Result::getColumn(const std::string &colName) {
        size_t colIndex = colNameToIndex[colName];
        return table[colIndex];
    }

    void Result::addRow(const std::vector<std::string> &row) {
        for (auto i = 0; i < row.size(); ++i) {
            table[i].push_back(row[i]);
        }
        empty = false;
    }

    std::vector<std::string>
    Result::joinRows(const std::vector<std::string> &row1,
                     const std::unordered_map<std::string, size_t> &colNamesToIndex1,
                     const std::vector<std::string> &row2,
                     const std::unordered_map<std::string, size_t> &colNamesToIndex2,
                     const std::unordered_map<std::string, size_t> &joinedColNamesToIndex) {
        std::vector<std::string> newRow = row1;
        newRow.reserve(joinedColNamesToIndex.size());
        for (size_t i = newRow.size(); i < joinedColNamesToIndex.size(); ++i) {
            newRow.emplace_back();
        }

        for (auto const &[colName, _]: colNamesToIndex2) {
            if (colNamesToIndex1.count(colName) < 1) {
                // row1 does not have this column from row2, add it
                size_t index = joinedColNamesToIndex.at(colName);
                newRow[index] = row2[colNamesToIndex2.at(colName)];
            }
        }

        return newRow;
    }

    std::unordered_map<std::string, size_t> Result::joinColNamesToIndex(
            const std::unordered_map<std::string, size_t> &colNamesToIndex1,
            const std::unordered_map<std::string, size_t> &colNamesToIndex2) {
        std::unordered_map<std::string, size_t> newColNamesToIndex = colNamesToIndex1;
        for (auto &[colName, _]: colNamesToIndex2) {
            if (newColNamesToIndex.count(colName) < 1) {
                newColNamesToIndex[colName] = newColNamesToIndex.size();
            }
        }

        return newColNamesToIndex;
    }

    std::vector<std::string> Result::getColumnNames(const std::unordered_map<std::string, size_t> &colNameToIndexMap) {
        std::vector<std::string> colNamesVector;
        colNamesVector.reserve(colNameToIndexMap.size());
        for (auto &[colName, _]: colNameToIndexMap) {
            colNamesVector.push_back(colName);
        }

        return colNamesVector;
    }
}