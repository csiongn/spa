#include "../lib/catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("With Evaluator") {
  SECTION("Single attribute ref") {
	SECTION("procedure.procName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: procedure p; Select p with p.procName = \"testProc1\";" << std::endl;

	  writer->insertProcedure("testProc1");
	  writer->insertProcedure("testProc2");

	  auto procedureSyn = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p", SimpleProgram::AttributeRef::NAME);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testProc1");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {procedureSyn, identSyn});
	  auto q = PQL::Query({procedureSyn}, {withClause}, {procedureSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testProc1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("call.procName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: call c; Select c with c.procName = \"testCall1\";" << std::endl;

	  writer->insertCallsProcStmt("testCall1", 1);
	  writer->insertCallsProcStmt("testCall1", 2);
	  writer->insertCallsProcStmt("testCall2", 3);

	  auto callSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c", SimpleProgram::AttributeRef::NAME);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testCall1");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {callSyn, identSyn});
	  auto q = PQL::Query({callSyn}, {withClause}, {callSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("variable.varName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: variable v; Select v with v.varName = \"testVar1\";" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");

	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v", SimpleProgram::AttributeRef::NAME);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {varSyn, identSyn});
	  auto q = PQL::Query({varSyn}, {withClause}, {varSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testVar1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("read.varName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: read r; Select r with r.varName = \"testVar1\";" << std::endl;

	  writer->insertReadVar("testVar1", 1);
	  writer->insertReadVar("testVar1", 2);
	  writer->insertReadVar("testVar2", 3);

	  auto readSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r", SimpleProgram::AttributeRef::NAME);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {readSyn, identSyn});
	  auto q = PQL::Query({readSyn}, {withClause}, {readSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("print.varName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: print pr; Select pr with pr.varName = \"testVar1\";" << std::endl;

	  writer->insertPrintVar("testVar1", 1);
	  writer->insertPrintVar("testVar1", 2);
	  writer->insertPrintVar("testVar2", 3);

	  auto printSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pr", SimpleProgram::AttributeRef::NAME);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {printSyn, identSyn});
	  auto q = PQL::Query({printSyn}, {withClause}, {printSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("constant.value") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: constant c; Select c with c.value = 3;" << std::endl;

	  writer->insertConstant({1, 2, 3});

	  auto constantSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "3");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {constantSyn, identSyn});
	  auto q = PQL::Query({constantSyn}, {withClause}, {constantSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"3"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("stmt.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: stmt s; Select s with s.stmt = 3;" << std::endl;

	  writer->insertStatement({1, 2, 3});

	  auto stmtSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s", SimpleProgram::AttributeRef::INTEGER);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "3");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {stmtSyn, identSyn});
	  auto q = PQL::Query({stmtSyn}, {withClause}, {stmtSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"3"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("read.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: read r; Select r with r.stmt = 3;" << std::endl;

	  writer->insertRead({1, 2, 3});

	  auto readSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r", SimpleProgram::AttributeRef::INTEGER);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "3");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {readSyn, identSyn});
	  auto q = PQL::Query({readSyn}, {withClause}, {readSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"3"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("print.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: print pr; Select pr with pr.stmt = 3;" << std::endl;

	  writer->insertPrint({1, 2, 3});

	  auto printSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pr", SimpleProgram::AttributeRef::INTEGER);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "3");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {printSyn, identSyn});
	  auto q = PQL::Query({printSyn}, {withClause}, {printSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"3"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("call.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: call c; Select c with c.stmt = 3;" << std::endl;

	  writer->insertCall({1, 2, 3});

	  auto callSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "3");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {callSyn, identSyn});
	  auto q = PQL::Query({callSyn}, {withClause}, {callSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"3"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("while.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: while w; Select w with w.stmt = 3;" << std::endl;

	  writer->insertWhile({1, 2, 3});

	  auto whileSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w", SimpleProgram::AttributeRef::INTEGER);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "3");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {whileSyn, identSyn});
	  auto q = PQL::Query({whileSyn}, {withClause}, {whileSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"3"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("if.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: if if; Select if with if.stmt = 3;" << std::endl;

	  writer->insertIf({1, 2, 3});

	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "if", SimpleProgram::AttributeRef::INTEGER);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "3");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {ifSyn, identSyn});
	  auto q = PQL::Query({ifSyn}, {withClause}, {ifSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"3"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("assign.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: assign a; Select a with a.stmt = 3;" << std::endl;

	  writer->insertAssign({1, 2, 3});

	  auto assignSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a", SimpleProgram::AttributeRef::INTEGER);
	  auto identSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "3");
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {assignSyn, identSyn});
	  auto q = PQL::Query({assignSyn}, {withClause}, {assignSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"3"};
	  REQUIRE(res == expectedRes);
	}
  }

  SECTION("Double attribute ref") {
	SECTION("procedure.procName and call.procName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: procedure p; call c; Select p with p.procName = c.procName;" << std::endl;

	  writer->insertProcedure("testProc1");
	  writer->insertProcedure("testProc2");
	  writer->insertCallsProcStmt("testProc1", 1);

	  auto procedureSyn = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p", SimpleProgram::AttributeRef::NAME);
	  auto callSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c", SimpleProgram::AttributeRef::NAME);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {procedureSyn, callSyn});
	  auto q = PQL::Query({procedureSyn, callSyn}, {withClause}, {procedureSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testProc1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("procedure.procName and var.procName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: procedure p; variable v; Select p with p.procName = v.procName;" << std::endl;

	  writer->insertProcedure("testIdent");
	  writer->insertProcedure("testProc2");
	  writer->insertVariable("testIdent");

	  auto procedureSyn = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p", SimpleProgram::AttributeRef::NAME);
	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v", SimpleProgram::AttributeRef::NAME);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {procedureSyn, varSyn});
	  auto q = PQL::Query({procedureSyn, varSyn}, {withClause}, {procedureSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testIdent"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("procedure.procName and read.varName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: procedure p; read r; Select p with p.procName = r.varName;" << std::endl;

	  writer->insertProcedure("testIdent");
	  writer->insertProcedure("testProc2");
	  writer->insertReadVar("testIdent", 1);

	  auto procedureSyn = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p", SimpleProgram::AttributeRef::NAME);
	  auto readSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r", SimpleProgram::AttributeRef::NAME);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {procedureSyn, readSyn});
	  auto q = PQL::Query({procedureSyn, readSyn}, {withClause}, {procedureSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testIdent"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("procedure.procName and print.varName") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: procedure p; print pn; Select p with p.procName = pn.varName;" << std::endl;

	  writer->insertProcedure("testIdent");
	  writer->insertProcedure("testProc2");
	  writer->insertPrintVar("testIdent", 1);

	  auto procedureSyn = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p", SimpleProgram::AttributeRef::NAME);
	  auto printSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn", SimpleProgram::AttributeRef::NAME);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {procedureSyn, printSyn});
	  auto q = PQL::Query({procedureSyn, printSyn}, {withClause}, {procedureSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testIdent"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("constant.value and stmt.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: constant c; stmt s; Select c with c.value = s.stmt;" << std::endl;

	  writer->insertConstant({1, 2, 3, 4, 5});
	  writer->insertStatement({1, 3, 5, 7, 9});

	  auto constSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto stmtSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {constSyn, stmtSyn});
	  auto q = PQL::Query({constSyn, stmtSyn}, {withClause}, {constSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "3", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("constant.value and read.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: constant c; read r; Select c with c.value = r.stmt;" << std::endl;

	  writer->insertConstant({1, 2, 3, 4, 5});
	  writer->insertRead({1, 3, 5, 7, 9});

	  auto constSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto readSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {constSyn, readSyn});
	  auto q = PQL::Query({constSyn, readSyn}, {withClause}, {constSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "3", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("constant.value and print.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: constant c; print pn; Select c with c.value = pn.stmt;" << std::endl;

	  writer->insertConstant({1, 2, 3, 4, 5});
	  writer->insertPrint({1, 3, 5, 7, 9});

	  auto constSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {constSyn, rSyn});
	  auto q = PQL::Query({constSyn, rSyn}, {withClause}, {constSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "3", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("constant.value and print.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: constant c; call call; Select c with c.value = call.stmt;" << std::endl;

	  writer->insertConstant({1, 2, 3, 4, 5});
	  writer->insertCall({1, 3, 5, 7, 9});

	  auto constSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "call", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {constSyn, rSyn});
	  auto q = PQL::Query({constSyn, rSyn}, {withClause}, {constSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "3", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("constant.value and while.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: constant c; while w; Select c with c.value = w.stmt;" << std::endl;

	  writer->insertConstant({1, 2, 3, 4, 5});
	  writer->insertWhile({1, 3, 5, 7, 9});

	  auto constSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {constSyn, rSyn});
	  auto q = PQL::Query({constSyn, rSyn}, {withClause}, {constSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "3", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("constant.value and if.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: constant c; if if; Select c with c.value = if.stmt;" << std::endl;

	  writer->insertConstant({1, 2, 3, 4, 5});
	  writer->insertIf({1, 3, 5, 7, 9});

	  auto constSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "if", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {constSyn, rSyn});
	  auto q = PQL::Query({constSyn, rSyn}, {withClause}, {constSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "3", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("constant.value and assign.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: constant c; assign a; Select c with c.value = a.stmt;" << std::endl;

	  writer->insertConstant({1, 2, 3, 4, 5});
	  writer->insertAssign({1, 3, 5, 7, 9});

	  auto constSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c", SimpleProgram::AttributeRef::INTEGER);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {constSyn, rSyn});
	  auto q = PQL::Query({constSyn, rSyn}, {withClause}, {constSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "3", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("stmt.stmt and assign.stmt") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: stmt s; assign a; Select s with s.stmt = a.stmt;" << std::endl;

	  writer->insertStatement({1, 2, 3, 4, 5});
	  writer->insertAssign({1, 3, 5, 7, 9});

	  auto lSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s", SimpleProgram::AttributeRef::INTEGER);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {lSyn, rSyn});
	  auto q = PQL::Query({lSyn, rSyn}, {withClause}, {lSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "3", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}
  }

  SECTION("No attribute ref") {
	SECTION("Integer empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: stmt s; Select s with 50 = 10;" << std::endl;

	  writer->insertStatement({1, 2, 3, 4, 5});

	  auto stmtSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
	  auto lSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "50", SimpleProgram::AttributeRef::INTEGER);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "10", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {lSyn, rSyn});
	  auto q = PQL::Query({stmtSyn}, {withClause}, {stmtSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("Integer non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << "Testing query: stmt s; Select s with 50 = 50;" << std::endl;

	  writer->insertStatement({1, 2, 3, 4, 5});

	  auto stmtSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
	  auto lSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "50", SimpleProgram::AttributeRef::INTEGER);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::INTEGER, "50", SimpleProgram::AttributeRef::INTEGER);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {lSyn, rSyn});
	  auto q = PQL::Query({stmtSyn}, {withClause}, {stmtSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("NAME empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << R"(Testing query: stmt s; Select s with "IDENT1" = "IDENT2")" << std::endl;

	  writer->insertStatement({1, 2, 3, 4, 5});

	  auto stmtSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
	  auto lSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "IDENT1", SimpleProgram::AttributeRef::NAME);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "IDENT2", SimpleProgram::AttributeRef::NAME);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {lSyn, rSyn});
	  auto q = PQL::Query({stmtSyn}, {withClause}, {stmtSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("NAME non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one WITH clause." << std::endl;
	  std::cout << R"(Testing query: stmt s; Select s with "IDENT1" = "IDENT1")" << std::endl;

	  writer->insertStatement({1, 2, 3, 4, 5});

	  auto stmtSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
	  auto lSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "IDENT1", SimpleProgram::AttributeRef::NAME);
	  auto rSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "IDENT1", SimpleProgram::AttributeRef::NAME);
	  auto withClause = PQL::Clause(SimpleProgram::DesignAbstraction::WITH, {lSyn, rSyn});
	  auto q = PQL::Query({stmtSyn}, {withClause}, {stmtSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(res == expectedRes);
	}
  }
}

