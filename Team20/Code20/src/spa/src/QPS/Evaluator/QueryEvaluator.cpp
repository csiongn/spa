#include <memory>
#include <string>
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

std::vector<std::string> QueryEvaluator::evaluateQuery(const PQL::Query &q) {
  initialiseDeclaration(q);

  for (const auto &clause : q.clauses) {
	auto isTrue = evaluateClause(clause);
	if (!isTrue) {
	  // empty result, final result will also be empty, can just return
	  if (q.selectSynonyms[0].entityType == SimpleProgram::DesignEntity::BOOLEAN) {
		return {"FALSE"};
	  }
	  return {};
	}
  }

  if (q.selectSynonyms[0].entityType == SimpleProgram::DesignEntity::BOOLEAN) {
	return {"TRUE"};
  }

  // need to select from the results
  std::vector<std::string> selectResult = resultStore->retrieveSelect(q.selectSynonyms);
  return selectResult;
}

bool QueryEvaluator::evaluateClause(const PQL::Clause &clause) {
  switch (clause.clauseType) {
	case SimpleProgram::DesignAbstraction::FOLLOWS:
	case SimpleProgram::DesignAbstraction::FOLLOWST:
	case SimpleProgram::DesignAbstraction::PARENT:
	case SimpleProgram::DesignAbstraction::PARENTT:
	case SimpleProgram::DesignAbstraction::NEXT:
	case SimpleProgram::DesignAbstraction::NEXTT:
	case SimpleProgram::DesignAbstraction::AFFECTS:
	  return StatementEvaluator{reader, clause, resultStore}.evaluate();
	case SimpleProgram::DesignAbstraction::USESS:
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	case SimpleProgram::DesignAbstraction::USESP:
	case SimpleProgram::DesignAbstraction::MODIFIESP:
	case SimpleProgram::DesignAbstraction::CALLS:
	case SimpleProgram::DesignAbstraction::CALLST:
	  return EntityEvaluator{reader, clause, resultStore}.evaluate();
	case SimpleProgram::DesignAbstraction::PATTERN_ASSIGN:
	  return AssignPatternEvaluator{reader, clause, resultStore}.evaluate();
	case SimpleProgram::DesignAbstraction::PATTERN_IF:
	case SimpleProgram::DesignAbstraction::PATTERN_WHILE:
	  return IfAndWhilePatternEvaluator(reader, clause, resultStore).evaluate();
	case SimpleProgram::DesignAbstraction::WITH:
	  return WithEvaluator(reader, clause, resultStore).evaluate();
	default:
	  return false;
  }
}

void QueryEvaluator::initialiseDeclaration(const PQL::Query &q) {
  // Add all declarations to the
  for (const auto &syn : q.declarations) {
	if (std::find(q.selectSynonyms.begin(), q.selectSynonyms.end(), syn) != q.selectSynonyms.end()) {
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
	case SimpleProgram::DesignEntity::READ:
	case SimpleProgram::DesignEntity::PRINT:
	case SimpleProgram::DesignEntity::ASSIGN:
	case SimpleProgram::DesignEntity::CALL:
	case SimpleProgram::DesignEntity::WHILE:
	case SimpleProgram::DesignEntity::IF:
	case SimpleProgram::DesignEntity::CONSTANT:
	  intRes = getIntResults(syn);
	  if (intRes.empty()) {
		return;
	  }
	  resultStore->createColumn(syn, intRes);
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

std::vector<int> QueryEvaluator::getIntResults(const PQL::Synonym &syn) const {
  switch (syn.entityType) {
	case SimpleProgram::DesignEntity::STMT:
	  return reader->getAllStatementNum();
	case SimpleProgram::DesignEntity::READ:
	  return reader->getAllReadStmtNum();
	case SimpleProgram::DesignEntity::PRINT:
	  return reader->getAllPrintStmtNum();
	case SimpleProgram::DesignEntity::ASSIGN:
	  return reader->getAllAssignStmtNum();
	case SimpleProgram::DesignEntity::CALL:
	  return reader->getAllCallStmtNum();
	case SimpleProgram::DesignEntity::WHILE:
	  return reader->getAllWhileStmtNum();
	case SimpleProgram::DesignEntity::IF:
	  return reader->getAllIfStmtNum();
	case SimpleProgram::DesignEntity::CONSTANT:
	  return reader->getAllConstants();
	default:
	  return {};
  }
}
}
