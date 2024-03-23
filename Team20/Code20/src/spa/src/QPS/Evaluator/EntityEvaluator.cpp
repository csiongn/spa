#include "EntityEvaluator.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace QueryEvaluator {

bool EntityEvaluator::evaluate() {
  // Handles clauses with entity reference: USESS/USESP/MODIFIESS/MODIFIESP

  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];

  if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO ||
	  lArg.entityType == SimpleProgram::DesignEntity::IDENT) {
	// USE & MODIFIES STATEMENT
	if (rArg.entityType == SimpleProgram::DesignEntity::IDENT) {
	  // 2 cases = (STMT_NO, IDENT), (IDENT, IDENT)
	  return hasRelationship();
	} else {
	  // 4 cases = (STMT_NO, SYN/_), (IDENT, SYN/_)
	  return getForwardRelationship();
	}
  }

  // 7 cases = (SYN, IDENT)
  if (rArg.entityType == SimpleProgram::DesignEntity::IDENT) {
	return getReversedRelationship();
  }

  // 7 cases = (SYN, _)
  if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	return getSynonymWildcard();
  }

  // 7 cases = (SYN, VAR)
  return getDoubleSynonym();
}

bool EntityEvaluator::isAlwaysFalse() {
  // no always false cases for Uses/UsesP/Modifies/ModifiesP
  return false;
}

bool EntityEvaluator::hasRelationship() {
  PQL::Synonym lArg = clause.arguments[0];
  std::string rIdent = clause.arguments[1].identity;

  if (lArg.entityType == SimpleProgram::DesignEntity::STMT_NO) {
	// handles XXX(STMT_NO, IDENT)
	int stmtNum = stoi(clause.arguments[0].identity);

	return hasRelationship(clause.clauseType, stmtNum, rIdent);
  } else {
	// handles XXX(IDENT, IDENT)
	std::string lIdent = clause.arguments[0].identity;

	return hasRelationship(clause.clauseType, lIdent, rIdent);
  }
}

bool EntityEvaluator::hasRelationship(const SimpleProgram::DesignAbstraction &relationship, int stmtNum,
									  const std::string &ident) {
  switch (relationship) {
	case SimpleProgram::DesignAbstraction::USESS:
	  return reader->containsUsesRelationship(stmtNum, ident);
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	  return reader->containsModifiesRelationship(stmtNum, ident);
	default:
	  // TODO: throw illegal argument, not allowed relationship type for queries with entity reference
	  return false;
  }
}

bool
EntityEvaluator::hasRelationship(const SimpleProgram::DesignAbstraction &relationship, const std::string &lIdent,
								 const std::string &rIdent) {
  switch (relationship) {
	case SimpleProgram::DesignAbstraction::USESP:
	  return reader->containsUsesProcRelationship(lIdent, rIdent);
	case SimpleProgram::DesignAbstraction::MODIFIESP:
	  return reader->containsModifiesProcRelationship(lIdent, rIdent);
	default:
	  // TODO: throw illegal argument, not allowed relationship type for queries with entity reference
	  return false;
  }
}

bool EntityEvaluator::hasAtLeastOneRelationship() {
  // handles XXX(_, _), which is invalid case
  return false;
}

bool EntityEvaluator::getForwardRelationship() {
  // handles XXX(STMT_NO, VAR), XXX(STMT_NO, _), XXX(IDENT, VAR), XXX(IDENT, _)
  return getRightResults();
}

bool EntityEvaluator::getRightResults() {
  // get all right var that satisfy the relationship with the fixed left stmtNum
  PQL::Synonym rArg = clause.arguments[1];

  std::vector<std::string> vars;
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::USESS:
	  vars = reader->getUsesVariable(stoi(clause.arguments[0].identity));
	  break;
	case SimpleProgram::DesignAbstraction::USESP:
	  vars = reader->getUsesProcVariable(clause.arguments[0].identity);
	  break;
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	  vars = reader->getModifiesVariable(stoi(clause.arguments[0].identity));
	  break;
	case SimpleProgram::DesignAbstraction::MODIFIESP:
	  vars = reader->getModifiesProcVariable(clause.arguments[0].identity);
	  break;
	default:
	  // TODO: throw illegal argument, not allowed relationship type for statement only queries
	  vars = {};
  }

  if (rArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	return !vars.empty();
  }

  if (vars.empty()) {
	return false;
  }

  resultStore->createColumn(rArg, vars);
  return true;
}

bool EntityEvaluator::getReversedRelationship() {
  // handles XXX(SYN, IDENT) and XXX(_, IDENT)
  return getLeftResults();
}

bool EntityEvaluator::getLeftResults() {
  // get all left stmtNums that satisfy the relationship with the fixed right ident
  PQL::Synonym lArg = clause.arguments[0];
  std::string ident = clause.arguments[1].identity;

  if (lArg.entityType == SimpleProgram::DesignEntity::PROCEDURE) {
	std::vector<std::string> procNames = getUniqueProcNames(ident);
	if (procNames.empty()) {
	  return false;
	}

	if (lArg.entityType != SimpleProgram::DesignEntity::WILDCARD) {
	  resultStore->createColumn(lArg, procNames);
	}
	return true;
  } else {
	std::vector<int> stmtNums;
	switch (clause.clauseType) {
	  case SimpleProgram::DesignAbstraction::USESS:
		stmtNums = reader->getUsesStmt(ident);
		break;
	  case SimpleProgram::DesignAbstraction::MODIFIESS:
		stmtNums = reader->getModifiesStmt(ident);
		break;
	  default:
		// TODO: throw illegal argument, not allowed relationship type for statement only queries
		stmtNums = {};
	}

	if (lArg.entityType == SimpleProgram::DesignEntity::WILDCARD) {
	  return !stmtNums.empty();
	}

	if (stmtNums.empty()) {
	  return false;
	}

	std::vector<int> synStmtNums = ClauseEvaluator::getStmtNums(lArg);
	std::vector<int> lResults = ClauseEvaluator::getIntersection(synStmtNums, stmtNums);

	if (lResults.empty()) {
	  return false;
	}

	resultStore->createColumn(lArg, lResults);
	return true;
  }
}

bool EntityEvaluator::getSynonymWildcard() {
  // handles (SYN, _)
  PQL::Synonym lArg = clause.arguments[0];
  if (lArg.entityType == SimpleProgram::DesignEntity::PROCEDURE) {
	std::vector<std::string> lResults = getUniqueProcNames();
	if (lResults.empty()) {
	  return false;
	}

	resultStore->createColumn(lArg, lResults);
	return true;
  } else {
	std::vector<int> lResults = getUniqueStmtNums(lArg);

	if (lResults.empty()) {
	  return false;
	}

	resultStore->createColumn(lArg, lResults);
	return true;
  }
}

bool EntityEvaluator::getWildcardSynonym() {
  // Handles XXX(_, VAR_SYN), which is invalid case
  return false;
}

bool EntityEvaluator::getDoubleSynonym() {
  // Handles (SYN, VAR)
  PQL::Synonym lArg = clause.arguments[0];
  PQL::Synonym rArg = clause.arguments[1];
  std::vector<std::string> var = getUniqueValues();
  std::vector<std::string> lResults = {};
  std::vector<std::string> rResults = {};
  if (lArg.entityType == SimpleProgram::DesignEntity::PROCEDURE) {
	std::vector<std::string> lValues = getUniqueProcNames();
	for (const auto &v1 : lValues) {
	  for (const auto &v2 : var) {
		if (hasRelationship(clause.clauseType, v1, v2)) {
		  lResults.push_back(v1);
		  rResults.push_back(v2);
		}
	  }
	}
  } else {
	std::vector<int> lValues = getUniqueStmtNums(lArg);
	for (auto v1 : lValues) {
	  for (const auto &v2 : var) {
		if (hasRelationship(clause.clauseType, v1, v2)) {
		  lResults.push_back(std::to_string(v1));
		  rResults.push_back(v2);
		}
	  }
	}
  }

  std::vector<std::vector<std::string>> table = {lResults, rResults};
  std::vector<std::string> colNames = {lArg.identity, rArg.identity};
  std::unordered_map<std::string, size_t> colNameToIndex;
  for (size_t i = 0; i < colNames.size(); i++) {
	colNameToIndex[colNames[i]] = i;
  }
  resultStore->insertResult(std::make_shared<Result>(table, colNames, colNameToIndex));
  return true;
}

std::vector<int> EntityEvaluator::getUniqueStmtNums(const PQL::Synonym &syn) {
  // get all possible stmtNum that satisfies the relationship
  std::vector<int> keyStmtNums;
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::USESS:
	  keyStmtNums = reader->getUsesStmt();
	  break;
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	  keyStmtNums = reader->getModifiesStmt();
	  break;
	default:
	  // TODO: throw illegal argument, not allowed relationship type for queries with entity reference
	  return {};
  }

  std::vector<int> synStmtNums = ClauseEvaluator::getStmtNums(syn);
  return ClauseEvaluator::getIntersection(keyStmtNums, synStmtNums);
}

std::vector<std::string> EntityEvaluator::getUniqueProcNames() {
  // get all possible procedure names
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::USESS:
	  return reader->getUsesProcName();
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	  return reader->getModifiesProcName();
	  break;
	default:
	  // TODO: throw illegal argument, not allowed relationship type for queries with entity reference
	  return {};
  }
}

std::vector<std::string> EntityEvaluator::getUniqueProcNames(const std::string &ident) {
  // get all possible procedure names that satisfies the relationship
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::USESS:
	  return reader->getUsesProcName(ident);
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	  return reader->getModifiesProcName(ident);
	  break;
	default:
	  // TODO: throw illegal argument, not allowed relationship type for queries with entity reference
	  return {};
  }
}

std::vector<std::string> EntityEvaluator::getUniqueValues() {
  // get all possible variables that satisfies the relationship
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::USESS:
	  return reader->getUsesVariable();
	case SimpleProgram::DesignAbstraction::USESP:
	  return reader->getUsesProcVariable();
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	  return reader->getModifiesVariable();
	case SimpleProgram::DesignAbstraction::MODIFIESP:
	  return reader->getModifiesProcVariable();
	default:
	  // TODO: throw illegal argument, not allowed relationship type for queries with entity reference
	  return {};
  }
}
}