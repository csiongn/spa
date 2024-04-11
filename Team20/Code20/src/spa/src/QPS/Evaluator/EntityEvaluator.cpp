#include "EntityEvaluator.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace QueryEvaluator {

bool EntityEvaluator::evaluate() {
  // Handles clauses with entity reference: USESS/USESP/MODIFIESS/MODIFIESP/CALLS/CALLST

  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];

  if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO ||
	  lArg.entityType == SimpleProgram::DesignEntity::IDENT) {
	if (rArg.entityType == SimpleProgram::DesignEntity::IDENT) {
	  // 2 cases = (STMT_NO, IDENT), (IDENT, IDENT)
	  return hasRelationship();
	} else {
	  // 4 cases = (STMT_NO, SYN/_), (IDENT, SYN/_)
	  return getForwardRelationship();
	}
  }

  if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	// Only for CALLS/CALLST (_, _) / (_, IDENT), (_, PROC_SYN)
	if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	  return hasAtLeastOneRelationship();
	} else if (rArg.entityType == SimpleProgram::DesignEntity::PROCEDURE) {
	  return getWildcardSynonym();
	} else {
	  return getReversedRelationship();
	}
  }

  std::unordered_map<SimpleProgram::DesignEntity, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignEntity::VARIABLE, [this] { return getDoubleSynonym(); }},
	  {SimpleProgram::DesignEntity::PROCEDURE, [this] { return getDoubleSynonym(); }},
	  {SimpleProgram::DesignEntity::WILDCARD, [this] { return getSynonymWildcard(); }},
	  {SimpleProgram::DesignEntity::IDENT, [this] { return getReversedRelationship(); }},
  };

  if (funcMap.find(rArg.entityType) == funcMap.end()) {
	// unhandled cases
	return false;
  }

  return funcMap[rArg.entityType]();
}

bool EntityEvaluator::isAlwaysFalse() {
  // no always false cases for Uses/UsesP/Modifies/ModifiesP
  return false;
}

bool EntityEvaluator::hasRelationship() {
  PQL::Synonym lArg = clause.arguments[0];
  std::string rIdent = clause.arguments[1].identity;
  bool isNotEmpty = false;

  if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
	// handles XXX(STMT_NO, IDENT) = USES & MODIFIES STATEMENT
	int stmtNum = stoi(clause.arguments[0].identity);

	isNotEmpty = hasRelationship(clause.clauseType, stmtNum, rIdent);
  } else {
	// handles XXX(IDENT, IDENT) = USESP & MODIFIESP & CALLS & CALLST
	std::string lIdent = clause.arguments[0].identity;

	isNotEmpty = hasRelationship(clause.clauseType, lIdent, rIdent);
  }

  if (clause.isNegated) {
	return !isNotEmpty;
  } else {
	return isNotEmpty;
  }
}

bool EntityEvaluator::hasRelationship(const SimpleProgram::DesignAbstraction &relationship, int stmtNum,
									  const std::string &ident) {
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::USESS,
	   [this, stmtNum, ident] { return reader->containsUsesRelationship(stmtNum, ident); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESS,
	   [this, stmtNum, ident] { return reader->containsModifiesRelationship(stmtNum, ident); }},
  };

  if (funcMap.find(relationship) == funcMap.end()) {
	return false;
  }

  return funcMap[relationship]();
}

bool
EntityEvaluator::hasRelationship(const SimpleProgram::DesignAbstraction &relationship, const std::string &lIdent,
								 const std::string &rIdent) {
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::USESP,
	   [this, lIdent, rIdent] { return reader->containsUsesProcRelationship(lIdent, rIdent); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESP,
	   [this, lIdent, rIdent] { return reader->containsModifiesProcRelationship(lIdent, rIdent); }},
	  {SimpleProgram::DesignAbstraction::CALLS,
	   [this, lIdent, rIdent] { return reader->containsCallsProcRelationship(lIdent, rIdent); }},
	  {SimpleProgram::DesignAbstraction::CALLST,
	   [this, lIdent, rIdent] { return reader->containsCallsTProcRelationship(lIdent, rIdent); }},
  };

  if (funcMap.find(relationship) == funcMap.end()) {
	return false;
  }

  return funcMap[relationship]();
}

bool EntityEvaluator::hasAtLeastOneRelationship() {
  // handles CALLS/CALLST (_, _),
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<bool()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::CALLS, [this] { return reader->hasCallsProcRelationship(); }},
	  {SimpleProgram::DesignAbstraction::CALLST, [this] { return reader->hasCallsTProcRelationship(); }},
  };

  bool isNotEmpty = false;
  if (funcMap.find(clause.clauseType) != funcMap.end()) {
	isNotEmpty = funcMap[clause.clauseType]();
  }

  if (clause.isNegated) {
	return !isNotEmpty;
  }

  return isNotEmpty;
}

bool EntityEvaluator::getForwardRelationship() {
  // handles XXX(STMT_NO, VAR), XXX(STMT_NO, _), XXX(IDENT, VAR), XXX(IDENT, _)
  return getRightResults();
}

bool EntityEvaluator::getRightResults() {
  // get all right var that satisfy the relationship with the fixed left STMT_NO/IDENT
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];

  std::vector<std::string> rResults = getUniqueValues(lArg);
  if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	if (clause.isNegated) {
	  return rResults.empty();
	} else {
	  return !rResults.empty();
	}
  }

  if (clause.isNegated) {
	rResults = negateStringResults(rArg, rResults);
  }

  if (rResults.empty()) {
	return false;
  }

  if (createTable) {
	resultStore->createColumn(rArg, rResults);
  }
  return true;
}

bool EntityEvaluator::getReversedRelationship() {
  // handles XXX(SYN, IDENT) and XXX(_, IDENT)
  return getLeftResults();
}

bool EntityEvaluator::getLeftResults() {
  // get all left STMT_NUM/IDENT that satisfy the relationship with the fixed right IDENT
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  std::string ident = clause.arguments[1].identity;

  if (lArg.entityType == SimpleProgram::DesignEntity::PROCEDURE
	  || lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	// USESP/MODIFIESP/CALLS/CALLST
	std::vector<std::string> procNames = getUniqueLeftProcNames(ident);

	if (clause.isNegated) {
	  procNames = negateStringResults(lArg, procNames);
	}

	if (procNames.empty()) {
	  return false;
	}

	if (lArg.entityType != SimpleProgram::DesignEntity::WILDCARD && createTable) {
	  resultStore->createColumn(lArg, procNames);
	}
	return true;
  } else {
	std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<int>()>> funcMap = {
		{SimpleProgram::DesignAbstraction::USESS, [this, ident] { return reader->getUsesStmt(ident); }},
		{SimpleProgram::DesignAbstraction::MODIFIESS, [this, ident] { return reader->getModifiesStmt(ident); }},
	};

	std::vector<int> stmtNums;
	if (funcMap.find(clause.clauseType) != funcMap.end()) {
	  stmtNums = funcMap[clause.clauseType]();
	}

	if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	  return !stmtNums.empty();
	}

	std::vector<int> synStmtNums = ClauseEvaluator::getAllIntResults(lArg);
	std::vector<int> lResults = ClauseEvaluator::getIntersection(synStmtNums, stmtNums);

	if (clause.isNegated) {
	  lResults = negateIntResults(lArg, lResults);
	}

	if (lResults.empty()) {
	  return false;
	}

	if (createTable) {
	  resultStore->createColumn(lArg, lResults);
	}
	return true;
  }
}

bool EntityEvaluator::getSynonymWildcard() {
  // handles (SYN, _)
  PQL::Synonym lArg = clause.arguments[0];
  if (lArg.entityType == SimpleProgram::DesignEntity::PROCEDURE) {
	std::vector<std::string> lResults = getUniqueLeftProcNames();

	if (clause.isNegated) {
	  lResults = negateStringResults(lArg, lResults);
	}

	if (lResults.empty()) {
	  return false;
	}

	if (createTable) {
	  resultStore->createColumn(lArg, lResults);
	}
	return true;
  } else {
	std::vector<int> lResults = getUniqueStmtNums(lArg);

	if (clause.isNegated) {
	  lResults = negateIntResults(lArg, lResults);
	}

	if (lResults.empty()) {
	  return false;
	}

	if (createTable) {
	  resultStore->createColumn(lArg, lResults);
	}
	return true;
  }
}

bool EntityEvaluator::getWildcardSynonym() {
  // Handles CALLS/CALLST (_, PROC_SYN)
  PQL::Synonym rArg = clause.arguments[1];
  std::vector<std::string> rResults = getUniqueValues();

  if (clause.isNegated) {
	rResults = negateStringResults(rArg, rResults);
  }
  if (rResults.empty()) {
	return false;
  }

  if (createTable) {
	resultStore->createColumn(rArg, rResults);
  }
  return true;
}

bool EntityEvaluator::getDoubleSynonym() {
  // Handles (SYN, VAR)
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  std::vector<std::string> values = getUniqueValues();
  std::vector<std::string> lResults = {};
  std::vector<std::string> rResults = {};
  if (lArg.entityType == SimpleProgram::DesignEntity::PROCEDURE) {
	std::vector<std::string> lValues = getUniqueLeftProcNames();
	for (const auto &v1 : lValues) {
	  for (const auto &v2 : values) {
		if (hasRelationship(clause.clauseType, v1, v2) || clause.isNegated) {
		  lResults.push_back(v1);
		  rResults.push_back(v2);
		}
	  }
	}
  } else {
	std::vector<int> lValues = getUniqueStmtNums(lArg);
	for (auto v1 : lValues) {
	  for (const auto &v2 : values) {
		if (hasRelationship(clause.clauseType, v1, v2) || clause.isNegated) {
		  lResults.push_back(std::to_string(v1));
		  rResults.push_back(v2);
		}
	  }
	}
  }

  if (lResults.empty()) {
	return false;
  }

  if (createTable) {
	std::vector<std::vector<std::string>> table = {lResults, rResults};
	std::vector<std::string> colNames = {lArg.identity, rArg.identity};
	std::unordered_map<std::string, size_t> colNameToIndex;
	for (size_t i = 0; i < colNames.size(); i++) {
	  colNameToIndex[colNames[i]] = i;
	}
	resultStore->insertResult(std::make_shared<Result>(table, colNames, colNameToIndex));
  }
  return true;
}

std::vector<int> EntityEvaluator::getUniqueStmtNums(const PQL::Synonym &syn) {
  // get all possible stmtNum that satisfies the relationship
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<int>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::USESS, [this] { return reader->getUsesStmt(); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESS, [this] { return reader->getModifiesStmt(); }},
  };

  std::vector<int> keyStmtNums;
  if (funcMap.find(clause.clauseType) != funcMap.end()) {
	keyStmtNums = funcMap[clause.clauseType]();
  }

  if (keyStmtNums.empty()) {
	return {};
  }

  std::vector<int> synStmtNums = ClauseEvaluator::getAllIntResults(syn);
  return ClauseEvaluator::getIntersection(keyStmtNums, synStmtNums);
}

std::vector<std::string> EntityEvaluator::getUniqueLeftProcNames() {
  // get all possible procedure names for left arg
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<std::string>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::USESP, [this] { return reader->getUsesProcName(); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESP, [this] { return reader->getModifiesProcName(); }},
	  {SimpleProgram::DesignAbstraction::CALLS, [this] { return reader->getCallsProcCaller(); }},
	  {SimpleProgram::DesignAbstraction::CALLST, [this] { return reader->getCallsTProcCaller(); }},
  };

  if (funcMap.find(clause.clauseType) == funcMap.end()) {
	return {};
  }

  return funcMap[clause.clauseType]();
}

std::vector<std::string> EntityEvaluator::getUniqueLeftProcNames(const std::string &ident) {
  // get all possible procedure names that satisfies the relationship for left arg
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<std::string>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::USESP, [this, ident] { return reader->getUsesProcName(ident); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESP, [this, ident] { return reader->getModifiesProcName(ident); }},
	  {SimpleProgram::DesignAbstraction::CALLS, [this, ident] { return reader->getCallsProcCaller(ident); }},
	  {SimpleProgram::DesignAbstraction::CALLST, [this, ident] { return reader->getCallsTProcCaller(ident); }},
  };

  if (funcMap.find(clause.clauseType) == funcMap.end()) {
	return {};
  }

  return funcMap[clause.clauseType]();
}

std::vector<std::string> EntityEvaluator::getUniqueValues() {
  // get all possible values that satisfies the relationship for right arg
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<std::string>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::USESS, [this] { return reader->getUsesVariable(); }},
	  {SimpleProgram::DesignAbstraction::USESP, [this] { return reader->getUsesProcVariable(); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESS, [this] { return reader->getModifiesVariable(); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESP, [this] { return reader->getModifiesProcVariable(); }},
	  {SimpleProgram::DesignAbstraction::CALLS, [this] { return reader->getCallsProcCallee(); }},
	  {SimpleProgram::DesignAbstraction::CALLST, [this] { return reader->getCallsTProcCallee(); }},
  };

  if (funcMap.find(clause.clauseType) == funcMap.end()) {
	return {};
  }

  return funcMap[clause.clauseType]();
}

std::vector<std::string> EntityEvaluator::getUniqueValues(const PQL::Synonym lArg) {
  // get all possible values that satisfies the relationship for right arg
  std::unordered_map<SimpleProgram::DesignAbstraction, std::function<std::vector<std::string>()>> funcMap = {
	  {SimpleProgram::DesignAbstraction::USESS, [this, lArg] { return reader->getUsesVariable(stoi(lArg.identity)); }},
	  {SimpleProgram::DesignAbstraction::USESP, [this, lArg] { return reader->getUsesProcVariable(lArg.identity); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESS,
	   [this, lArg] { return reader->getModifiesVariable(stoi(lArg.identity)); }},
	  {SimpleProgram::DesignAbstraction::MODIFIESP,
	   [this, lArg] { return reader->getModifiesProcVariable(lArg.identity); }},
	  {SimpleProgram::DesignAbstraction::CALLS, [this, lArg] { return reader->getCallsProcCallee(lArg.identity); }},
	  {SimpleProgram::DesignAbstraction::CALLST, [this, lArg] { return reader->getCallsTProcCallee(lArg.identity); }},
  };

  if (funcMap.find(clause.clauseType) == funcMap.end()) {
	return {};
  }

  return funcMap[clause.clauseType]();
}

std::vector<std::string> EntityEvaluator::negateStringResults(const PQL::Synonym &syn,
															  const std::vector<std::string> &selected) {
  std::vector<std::string> stringResults = getUniqueIdent(syn);
  std::vector<std::string> negatedResults;
  std::unordered_set<std::string> selectedSet(selected.begin(), selected.end());

  for (auto const &val : stringResults) {
	if (selectedSet.find(val) == selectedSet.end()) {
	  negatedResults.push_back(val);
	}
  }

  return negatedResults;
}

std::vector<std::string> EntityEvaluator::getUniqueIdent(const PQL::Synonym &syn) {
  std::unordered_map<SimpleProgram::DesignEntity, std::function<std::vector<std::string>()>> funcMap = {
	  {SimpleProgram::DesignEntity::VARIABLE, [this] { return reader->getAllVariables(); }},
	  {SimpleProgram::DesignEntity::PROCEDURE, [this] { return reader->getAllVariables(); }},
	  {SimpleProgram::DesignEntity::WILDCARD, [this] { return reader->getAllVariables(); }},
  };

  if (funcMap.find(syn.entityType) == funcMap.end()) {
	return {};
  }

  return funcMap[syn.entityType]();
}
}