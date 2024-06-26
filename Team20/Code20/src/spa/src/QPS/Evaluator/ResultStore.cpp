#include "ResultStore.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../Utils/EvaluatorUtils.h"

namespace QueryEvaluator {
void ResultStore::insertResult(std::shared_ptr<Result> res) {
  results.push_back(res);
  for (auto &colName : res->colNames) {
	addedIdents.insert(colName);
  }
}

std::vector<std::string> ResultStore::retrieveSelect(const std::vector<PQL::Synonym> &selectSyns) {
  bool selectBoolean = selectSyns[0].entityType == SimpleProgram::DesignEntity::BOOLEAN;
  if (results.empty()) {
	if (selectBoolean) {
	  // All clauses are T/F clauses and there is no result added/initialised
	  return {"TRUE"};
	}
	return {};
  }

  joinResults();
  if (results.empty()) {
	if (selectBoolean) {
	  // result after joining became empty
	  return {"FALSE"};
	}
	return {};
  }

  std::shared_ptr<Result> res = results[0];
  if (selectSyns.size() == 1) {
	if (selectBoolean) {
	  return {"TRUE"};
	}

	size_t selectSynIndex = res->colNameToIndex.at(getSelectColName(selectSyns[0]));
	std::vector<std::string> selectResult = res->table[selectSynIndex];
	return removeDuplicates(selectResult);
  } else {
	// tuple
	std::vector<size_t> selectSynIndexes;
	selectSynIndexes.reserve(selectSyns.size());
	for (const PQL::Synonym &syn : selectSyns) {
	  selectSynIndexes.push_back(res->colNameToIndex.at(getSelectColName(syn)));
	}

	std::unordered_set<std::vector<std::string>, EvaluatorUtils::vectorHash> addedRows;
	std::vector<std::string> selectResult;
	size_t numRows = res->table[0].size();
	selectResult.reserve(numRows);
	for (size_t rowIndex = 0; rowIndex < numRows; rowIndex++) {
	  std::vector<std::string> row = getSelectedColsAtRow(res, selectSynIndexes, rowIndex);
	  if (addedRows.find(row) != addedRows.end()) {
		continue;
	  }

	  std::string curr;
	  for (size_t ind = 0; ind < row.size(); ind++) {
		curr += row[ind];
		if (ind != selectSynIndexes.size() - 1) {
		  curr += " ";
		}
	  }
	  selectResult.push_back(curr);
	  addedRows.insert(row);
	}
	return selectResult;
  }
}

void ResultStore::joinResults() {
  std::shared_ptr<Result> t1;
  std::shared_ptr<Result> t2;
  std::vector<std::string> commonCols;
  std::shared_ptr<Result> newResult;
  while (results.size() > 1) {
	t1 = results[0];
	t2 = results[1];
	commonCols = t1->getCommonColumns(*t2);
	if (commonCols.empty()) {
	  // cross join
	  newResult = t1->crossJoin(*t2);
	} else {
	  // hash join
	  newResult = t1->hashJoin(*t2, commonCols);
	  if (newResult->empty) {
		results.clear();
		return;
	  }
	}

	results.erase(results.begin());
	results.erase(results.begin());
	results.insert(results.begin(), newResult);
  }
}

std::vector<std::string> ResultStore::removeDuplicates(const std::vector<std::string> &column) {
  std::unordered_set<std::string> added;
  std::vector<std::string> noDuplicates;
  for (auto &value : column) {
	if (added.count(value) > 0) {
	  continue;
	}
	added.insert(value);
	noDuplicates.push_back(value);
  }

  return noDuplicates;
}

std::string ResultStore::getSelectColName(const PQL::Synonym &selectSyn) {
  std::unordered_map<SimpleProgram::DesignEntity, std::string> attrRefMap = {
	  {SimpleProgram::DesignEntity::CALL, "procName"},
	  {SimpleProgram::DesignEntity::READ, "varName"},
	  {SimpleProgram::DesignEntity::PRINT, "varName"},
  };
  switch (selectSyn.entityType) {
	case SimpleProgram::DesignEntity::CALL:
	case SimpleProgram::DesignEntity::READ:
	case SimpleProgram::DesignEntity::PRINT:
	  if (selectSyn.attribute == SimpleProgram::AttributeRef::NAME) {
		return selectSyn.identity + "." + attrRefMap[selectSyn.entityType];
	  }
	default:
	  return selectSyn.identity;
  }
}

bool ResultStore::containsColumn(const std::string &ident) {
  return addedIdents.find(ident) != addedIdents.end();
}

std::vector<std::string> ResultStore::getSelectedColsAtRow(std::shared_ptr<Result> res,
														   const std::vector<size_t> &selectSynIndexes,
														   size_t rowIndex) {
  std::vector<std::string> row;
  for (auto colIndex : selectSynIndexes) {
	row.push_back(res->table[colIndex][rowIndex]);
  }
  return row;
}
}
