#include "../lib/catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("Entity Evaluator") {
  SECTION("Evaluate entity evaluator with STMT_NUM as left argument") {
	SECTION("VAR_SYN right argument non-empty and correct variables") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: VAR_SYN" << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, v);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertModifiesStmt(2, "testVar1");
	  writer->insertUsesStmt(2, "testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");

	  PQL::Clause usesClause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {lArgSyn, rArgSyn});
	  PQL::Clause modifiesClause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {lArgSyn, rArgSyn});
	  std::vector<PQL::Clause> clauses = {usesClause, modifiesClause};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {usesClause}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &cl : clauses) {
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::USESS:
			std::cout << "TESTING USESS" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::MODIFIESS:
			std::cout << "TESTING MODIFIESS" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}

		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
	  }
	}

	SECTION("VAR_SYN right argument empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: VAR_SYN" << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, v);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");

	  PQL::Clause usesClause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {lArgSyn, rArgSyn});
	  PQL::Clause modifiesClause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {lArgSyn, rArgSyn});
	  std::vector<PQL::Clause> clauses = {usesClause, modifiesClause};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {usesClause}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &cl : clauses) {
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::USESS:
			std::cout << "TESTING USESS" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::MODIFIESS:
			std::cout << "TESTING MODIFIESS" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}

		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
	  }
	}

	SECTION("WILDCARD right argument non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertModifiesStmt(2, "testVar1");
	  writer->insertUsesStmt(2, "testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  PQL::Clause usesClause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {lArgSyn, rArgSyn});
	  PQL::Clause modifiesClause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {lArgSyn, rArgSyn});
	  std::vector<PQL::Clause> clauses = {usesClause, modifiesClause};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {usesClause}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1", "testVar2"};
	  for (auto const &cl : clauses) {
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::USESS:
			std::cout << "TESTING USESS" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::MODIFIESS:
			std::cout << "TESTING MODIFIESS" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}

		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(checkVecValuesEqual(res, expectedRes));
	  }
	}

	SECTION("WILDCARD right argument empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  PQL::Clause usesClause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {lArgSyn, rArgSyn});
	  PQL::Clause modifiesClause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {lArgSyn, rArgSyn});
	  std::vector<PQL::Clause> clauses = {usesClause, modifiesClause};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {usesClause}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &cl : clauses) {
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::USESS:
			std::cout << "TESTING USESS" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::MODIFIESS:
			std::cout << "TESTING MODIFIESS" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}

		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(checkVecValuesEqual(res, expectedRes));
	  }
	}

	SECTION("IDENT right argument non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: IDENT" << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, TEST_IDENT);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertUsesStmt(2, "testVar1");
	  writer->insertModifiesStmt(2, "testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");

	  PQL::Clause usesClause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {lArgSyn, rArgSyn});
	  PQL::Clause modifiesClause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {lArgSyn, rArgSyn});
	  std::vector<PQL::Clause> clauses = {usesClause, modifiesClause};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {usesClause}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1", "testVar2"};
	  for (auto const &cl : clauses) {
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::USESS:
			std::cout << "TESTING USESS" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::MODIFIESS:
			std::cout << "TESTING MODIFIESS" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}

		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		bool equal = checkVecValuesEqual(res, expectedRes);
//                REQUIRE(checkVecValuesEqual(res, expectedRes));
		REQUIRE(equal == true);
	  }
	}

	SECTION("IDENT right argument empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: IDENT" << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, TEST_IDENT);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");

	  PQL::Clause usesClause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {lArgSyn, rArgSyn});
	  PQL::Clause modifiesClause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {lArgSyn, rArgSyn});
	  std::vector<PQL::Clause> clauses = {usesClause, modifiesClause};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {usesClause}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &cl : clauses) {
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::USESS:
			std::cout << "TESTING USESS" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::MODIFIESS:
			std::cout << "TESTING MODIFIESS" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}

		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
	  }
	}
  }

  SECTION("Evaluate entity evaluator with SYN as left argument") {
	SECTION("VAR_SYN right argument non-empty and correct variables") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: SYN, rArg: VAR_SYN" << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(SYN, v);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertModifiesStmt(3, "testVar1");
	  writer->insertUsesStmt(3, "testVar1");
	  writer->insertStatement(3);
	  writer->insertRead(3);
	  writer->insertPrint(3);
	  writer->insertAssign(3);
	  writer->insertCall(3);
	  writer->insertWhile(3);
	  writer->insertIf(3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");

	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {rArgSyn, rArgSyn})};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &syn : declarations) {
		clauses = {PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {syn, rArgSyn}),
				   PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::USESS:
			  std::cout << "TESTING USESS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::MODIFIESS:
			  std::cout << "TESTING MODIFIESS" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("VAR_SYN right argument non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: SYN, rArg: VAR_SYN" << std::endl;
	  std::cout << "Testing query: variable v; Select SYN such that XXX(SYN, v);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertModifiesStmt(3, "testVar1");
	  writer->insertUsesStmt(3, "testVar1");
	  writer->insertStatement(3);
	  writer->insertRead(3);
	  writer->insertPrint(3);
	  writer->insertAssign(3);
	  writer->insertCall(3);
	  writer->insertWhile(3);
	  writer->insertIf(3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");

	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {rArgSyn, rArgSyn})};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"3"};
	  for (auto const &syn : declarations) {
		clauses = {PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {syn, rArgSyn}),
				   PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::USESS:
			  std::cout << "TESTING USESS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::MODIFIESS:
			  std::cout << "TESTING MODIFIESS" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("VAR_SYN right argument empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: SYN, rArg: VAR_SYN" << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(SYN, v);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertStatement(3);
	  writer->insertRead(3);
	  writer->insertPrint(3);
	  writer->insertAssign(3);
	  writer->insertCall(3);
	  writer->insertWhile(3);
	  writer->insertIf(3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");

	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {rArgSyn, rArgSyn})};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &syn : declarations) {
		clauses = {PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {syn, rArgSyn}),
				   PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::USESS:
			  std::cout << "TESTING USESS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::MODIFIESS:
			  std::cout << "TESTING MODIFIESS" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("WILDCARD right argument non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: VAR_SYN" << std::endl;
	  std::cout << "Testing query: SYN_S; Select SYN such that XXX(SYN, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertModifiesStmt(3, "testVar1");
	  writer->insertUsesStmt(3, "testVar1");
	  writer->insertStatement(3);
	  writer->insertRead(3);
	  writer->insertPrint(3);
	  writer->insertAssign(3);
	  writer->insertCall(3);
	  writer->insertWhile(3);
	  writer->insertIf(3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {rArgSyn, rArgSyn})};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"3"};
	  for (auto const &syn : declarations) {
		clauses = {PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {syn, rArgSyn}),
				   PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::USESS:
			  std::cout << "TESTING USESS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::MODIFIESS:
			  std::cout << "TESTING MODIFIESS" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("WILDCARD right argument empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: SYN, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query: SYN_S; Select SYN such that XXX(SYN, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {rArgSyn, rArgSyn})};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &syn : declarations) {
		clauses = {PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {syn, rArgSyn}),
				   PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::USESS:
			  std::cout << "TESTING USESS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::MODIFIESS:
			  std::cout << "TESTING MODIFIESS" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("IDENT right argument non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: SYN, rArg: IDENT" << std::endl;
	  std::cout << "Testing query: SYN_S; Select SYN such that XXX(SYN, TEST_IDENT);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertModifiesStmt(3, "testVar1");
	  writer->insertUsesStmt(3, "testVar1");
	  writer->insertStatement(3);
	  writer->insertRead(3);
	  writer->insertPrint(3);
	  writer->insertAssign(3);
	  writer->insertCall(3);
	  writer->insertWhile(3);
	  writer->insertIf(3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");

	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {rArgSyn, rArgSyn})};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"3"};
	  for (auto const &syn : declarations) {
		clauses = {PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {syn, rArgSyn}),
				   PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::USESS:
			  std::cout << "TESTING USESS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::MODIFIESS:
			  std::cout << "TESTING MODIFIESS" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("IDENT right argument empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: SYN, rArg: IDENT" << std::endl;
	  std::cout << "Testing query: SYN_S; Select SYN such that XXX(SYN, TEST_IDENT);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");

	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {rArgSyn, rArgSyn})};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &syn : declarations) {
		clauses = {PQL::Clause(SimpleProgram::DesignAbstraction::USESS, {syn, rArgSyn}),
				   PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::USESS:
			  std::cout << "TESTING USESS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::MODIFIESS:
			  std::cout << "TESTING MODIFIESS" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}
  }
}