#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/facade/IPKBReader.h"
#include "QueryEvaluator.h"
#include "ResultStore.h"
#include "StatementEvaluator.h"
#include "EntityEvaluator.h"
#include "AssignPatternEvaluator.h"
#include "IfAndWhilePatternEvaluator.h"
#include "WithEvaluator.h"

namespace QueryEvaluator {
QueryEvaluator::QueryEvaluator(std::shared_ptr<IPKBReader> r) : resultStore(std::make_shared<ResultStore>()),
																reader(r) {}

std::vector<std::string> QueryEvaluator::evaluateQuery(PQL::Query &q) {
  initialiseDeclaration(q);
  std::vector<std::pair<PQL::Clause, bool>> sortedClauses = sortClauses(q);

  for (const auto &[clause, createTable] : sortedClauses) {
	auto isTrue = evaluateClause(clause, createTable);
	if (!isTrue) {
	  // empty result, final result will also be empty, can just return
	  if (q.selectSynonyms[0].entityType == SimpleProgram::DesignEntity::BOOLEAN) {
		return {"FALSE"};
	  }
	  return {};
	}
  }

  // all clauses evaluate to non-empty/TRUE
  // need to select from the results
  std::vector<std::string> selectResult = resultStore->retrieveSelect(q.selectSynonyms);
  return selectResult;
}

bool QueryEvaluator::evaluateClause(const PQL::Clause &clause, bool createTable) {
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::FOLLOWS:
	case SimpleProgram::DesignAbstraction::FOLLOWST:
	case SimpleProgram::DesignAbstraction::PARENT:
	case SimpleProgram::DesignAbstraction::PARENTT:
	case SimpleProgram::DesignAbstraction::NEXT:
	case SimpleProgram::DesignAbstraction::NEXTT:
	case SimpleProgram::DesignAbstraction::AFFECTS:
	  return StatementEvaluator{reader, clause, resultStore, createTable}.evaluate();
	case SimpleProgram::DesignAbstraction::USESS:
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	case SimpleProgram::DesignAbstraction::USESP:
	case SimpleProgram::DesignAbstraction::MODIFIESP:
	case SimpleProgram::DesignAbstraction::CALLS:
	case SimpleProgram::DesignAbstraction::CALLST:
	  return EntityEvaluator{reader, clause, resultStore, createTable}.evaluate();
	case SimpleProgram::DesignAbstraction::PATTERN_ASSIGN:
	  return AssignPatternEvaluator{reader, clause, resultStore, createTable}.evaluate();
	case SimpleProgram::DesignAbstraction::PATTERN_IF:
	case SimpleProgram::DesignAbstraction::PATTERN_WHILE:
	  return IfAndWhilePatternEvaluator{reader, clause, resultStore, createTable}.evaluate();
	case SimpleProgram::DesignAbstraction::WITH:
	  return WithEvaluator{reader, clause, resultStore, createTable}.evaluate();
	default:
	  return false;
  }
}

void QueryEvaluator::initialiseDeclaration(const PQL::Query &q) {
  // Add all declarations to the
  for (const auto &syn : q.declarations) {
	if (std::any_of(q.selectSynonyms.begin(),
					q.selectSynonyms.end(),
					[syn](const PQL::Synonym& selectSyn)->bool{ return syn.identity == selectSyn.identity; })) {
	  addSynonymToStore(syn);
	}
  }
}

void QueryEvaluator::addSynonymToStore(const PQL::Synonym &syn) {
  std::vector<std::string> strRes;
  std::vector<int> intRes;
  switch (syn.entityType) {
	case SimpleProgram::DesignEntity::PROCEDURE:
	case SimpleProgram::DesignEntity::VARIABLE:
	  strRes = getStringResults(syn);
	  if (strRes.empty()) {
		return;
	  }
	  resultStore->createColumn(syn, strRes);
	  return;

	case SimpleProgram::DesignEntity::STMT:
	case SimpleProgram::DesignEntity::ASSIGN:
	case SimpleProgram::DesignEntity::WHILE:
	case SimpleProgram::DesignEntity::IF:
	case SimpleProgram::DesignEntity::CONSTANT:
	  intRes = getIntResults(syn);
	  if (intRes.empty()) {
		return;
	  }
	  resultStore->createColumn(syn, intRes);
	  return;

	case SimpleProgram::DesignEntity::CALL:
	case SimpleProgram::DesignEntity::READ:
	case SimpleProgram::DesignEntity::PRINT:
	  initialiseDoubleColumn(syn);
	  return;
	default:
	  return;
  }
}

std::vector<std::string> QueryEvaluator::getStringResults(const PQL::Synonym &syn) const {
  switch (syn.entityType) {
	case SimpleProgram::DesignEntity::PROCEDURE:
	  return reader->getAllProcedures();
	case SimpleProgram::DesignEntity::VARIABLE:
	  return reader->getAllVariables();
	default:
	  return {};
  }
}

std::vector<std::string> QueryEvaluator::getStringResults(const PQL::Synonym &syn, int stmtNum) const {
  switch (syn.entityType) {
	case SimpleProgram::DesignEntity::CALL:
	  return reader->getCallsProcName(stmtNum);
	case SimpleProgram::DesignEntity::READ:
	  return reader->getReadVariable(stmtNum);
	case SimpleProgram::DesignEntity::PRINT:
	  return reader->getPrintVariable(stmtNum);
	default:
	  return {};
  }
}

std::vector<int> QueryEvaluator::getIntResults(const PQL::Synonym &syn) const {
  std::unordered_map<SimpleProgram::DesignEntity, std::function<std::vector<int>()>> funcMap = {
	  {SimpleProgram::DesignEntity::STMT, [this] { return reader->getAllStatementNum(); }},
	  {SimpleProgram::DesignEntity::READ, [this] { return reader->getAllReadStmtNum(); }},
	  {SimpleProgram::DesignEntity::PRINT, [this] { return reader->getAllPrintStmtNum(); }},
	  {SimpleProgram::DesignEntity::ASSIGN, [this] { return reader->getAllAssignStmtNum(); }},
	  {SimpleProgram::DesignEntity::CALL, [this] { return reader->getAllCallStmtNum(); }},
	  {SimpleProgram::DesignEntity::WHILE, [this] { return reader->getAllWhileStmtNum(); }},
	  {SimpleProgram::DesignEntity::IF, [this] { return reader->getAllIfStmtNum(); }},
	  {SimpleProgram::DesignEntity::CONSTANT, [this] { return reader->getAllConstants(); }}
  };

  if (funcMap.find(syn.entityType) == funcMap.end()) {
	return {};
  }

  return funcMap[syn.entityType]();
}

std::vector<std::pair<PQL::Clause, bool>> QueryEvaluator::sortClauses(PQL::Query &q) {
  std::unordered_set<int> clauseIndexSet;
  clauseIndexSet.reserve(q.clauses.size());

  std::vector<std::pair<PQL::Clause, bool>> sortedClauses;
  sortedClauses.reserve(q.clauses.size());

  // find clauses without synonyms
  for (int i = 0; i < q.clauses.size(); ++i) {
	if (hasNoSynonym(q.clauses[i])) {
	  sortedClauses.emplace_back(q.clauses[i], false);
	  clauseIndexSet.insert(i);
	}
  }

  // group clauses by their synonyms
  std::vector<std::pair<std::unordered_set<std::string>, std::vector<PQL::Clause>>>
	  synClausePairs = groupClauses(q.clauses, clauseIndexSet);
  std::vector<std::string> selectSynonymsIdent;
  selectSynonymsIdent.reserve(q.selectSynonyms.size());
  for (const PQL::Synonym &syn : q.selectSynonyms) {
	selectSynonymsIdent.push_back(syn.identity);
  }
  for (const auto &[identities, cls] : synClausePairs) {
	// if <= 1, means the synonym is only used by this clause, means it's equivalent to a true/false clause
	// then check if any of the identities is selected, if yes, need to add to filter the initialised result
	bool createTable = cls.size() > 1 || hasIntersection(identities, selectSynonymsIdent);
	for (const PQL::Clause &cl : cls) {
	  sortedClauses.emplace_back(cl, createTable);
	}
  }

  return sortedClauses;
}

bool QueryEvaluator::hasNoSynonym(PQL::Clause &cl) {
  std::unordered_set<SimpleProgram::DesignAbstraction> clausesWithSynonym = {
	  SimpleProgram::DesignAbstraction::PATTERN_ASSIGN,
	  SimpleProgram::DesignAbstraction::PATTERN_IF,
	  SimpleProgram::DesignAbstraction::PATTERN_WHILE,
  };

  if (clausesWithSynonym.find(cl.clauseType) != clausesWithSynonym.end()) {
	return false;
  }

  // non-pattern clause, only have two arguments, check both
  return !isSynonym(cl.arguments[0]) && !isSynonym(cl.arguments[1]);
}

bool QueryEvaluator::isSynonym(const PQL::Synonym &arg) {
  std::unordered_set<SimpleProgram::DesignEntity> nonSynonymEntities = {
	  SimpleProgram::DesignEntity::STMT_NO,
	  SimpleProgram::DesignEntity::INTEGER,
	  SimpleProgram::DesignEntity::IDENT,
	  SimpleProgram::DesignEntity::EXPR,
	  SimpleProgram::DesignEntity::PARTIAL_EXPR,
	  SimpleProgram::DesignEntity::WILDCARD,
  };

  return nonSynonymEntities.find(arg.entityType) == nonSynonymEntities.end();
}

std::vector<std::pair<std::unordered_set<std::string>,
					  std::vector<PQL::Clause>>> QueryEvaluator::groupClauses(const std::vector<PQL::Clause> &cls,
																			  const std::unordered_set<int> &noSynonymClauses) {
  std::unordered_map<int, PQL::Clause> idMap;
  int i = 0;
  for (const PQL::Clause &cl : cls) {
	idMap.insert({i++, cl});
  }

  std::unordered_map<std::unordered_set<std::string>, std::unordered_set<int>, SetHash> synToClauseIdMap;
  for (i = 0; i < cls.size(); ++i) {
	if (noSynonymClauses.find(i) != noSynonymClauses.end()) {
	  continue;
	}

	for (const PQL::Synonym &arg : cls[i].arguments) {
	  if (!isSynonym(arg)) {
		continue;
	  }

	  synToClauseIdMap[{arg.identity}].insert(i);
	}
  }

  mergeKeyValuePair(synToClauseIdMap);

  std::vector<std::pair<std::unordered_set<std::string>, std::vector<PQL::Clause>>> synClausePairs;
  for (const auto &[id, set] : synToClauseIdMap) {
	std::vector<PQL::Clause> clsGroup;
	for (int synId : set) {
	  clsGroup.emplace_back(idMap.at(synId));
	}

	synClausePairs.emplace_back(id, clsGroup);
  }
  return synClausePairs;
}

bool QueryEvaluator::hasIntersection(const std::unordered_set<int> &s1, const std::unordered_set<int> &s2) {
  std::unordered_set<int> smaller = s1;
  std::unordered_set<int> bigger = s2;
  if (s1.size() > s2.size()) {
	std::swap(smaller, bigger);
  }

  return std::any_of(smaller.begin(), smaller.end(),
					 [bigger](int id) { return bigger.find(id) != bigger.end(); });
}

bool QueryEvaluator::hasIntersection(const std::unordered_set<std::string> &set, const std::vector<std::string> &vec) {
  for (const std::string &s : vec) {
	if (set.find(s) != set.end()) {
	  return true;
	}
  }

  return false;
}

void QueryEvaluator::mergeKeyValuePair(std::unordered_map<std::unordered_set<std::string>,
														  std::unordered_set<int>,
														  SetHash> &map) {
  for (auto it1 = map.begin(); it1 != map.end(); ++it1) {
	for (auto it2 = map.begin(); it2 != map.end(); ++it2) {
	  if (it1 == it2 || !hasIntersection(it1->second, it2->second)) {
		continue;
	  }

	  // has intersection in value, merge the new key and value pair
	  std::unordered_set<std::string> newKey = it1->first;
	  newKey.insert(it2->first.begin(), it2->first.end());
	  std::unordered_set<int> newValues = it1->second;
	  newValues.insert(it2->second.begin(), it2->second.end());

	  // remove the old key-value pair
	  map.erase(it1);
	  map.erase(it2);

	  // insert the new key-value pair and recursively merge the map
	  map.insert({newKey, newValues});
	  mergeKeyValuePair(map);

	  return;
	}
  }
}

void QueryEvaluator::initialiseDoubleColumn(const PQL::Synonym &syn) {
  std::vector<int> intRes = getIntResults(syn);
  if (intRes.empty()) {
	return;
  }

  std::vector<std::pair<std::string, std::string>> results;
  for (int stmtNum : intRes) {
	std::vector<std::string> strRes = getStringResults(syn, stmtNum);
	for (const std::string &ident : strRes) {
	  results.emplace_back(std::to_string(stmtNum), ident);
	}
  }

  std::vector<std::string> lResults;
  std::vector<std::string> rResults;
  for (const auto &[stmtNum, ident] : results) {
	lResults.emplace_back(stmtNum);
	rResults.emplace_back(ident);
  }

  std::unordered_map<SimpleProgram::DesignEntity, std::string> attrRefMap = {
	  {SimpleProgram::DesignEntity::CALL, "procName"},
	  {SimpleProgram::DesignEntity::READ, "varName"},
	  {SimpleProgram::DesignEntity::PRINT, "varName"},
  };

  std::vector<std::vector<std::string>> table = {lResults, rResults};
  std::string attrRefColName = syn.identity + "." + attrRefMap[syn.entityType];
  std::vector<std::string> colNames = {syn.identity, attrRefColName};
  std::unordered_map<std::string, size_t> colNameToIndex;
  for (size_t i = 0; i < colNames.size(); i++) {
	colNameToIndex[colNames[i]] = i;
  }
  resultStore->insertResult(std::make_shared<Result>(table, colNames, colNameToIndex));
}
}
