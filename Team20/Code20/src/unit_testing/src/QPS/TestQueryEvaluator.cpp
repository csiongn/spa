#include "../lib/catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("Query Evaluator") {
  SECTION("Evaluate query with only select") {
	SECTION("one variable only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one variable." << std::endl;
	  std::cout << "Testing query: variable v; Select v;" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto q = PQL::Query({varDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testVar1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple variables") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple variables." << std::endl;
	  std::cout << "Testing query: variable v; Select v;" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertVariable("testVar3");
	  writer->insertVariable("testVar4");
	  writer->insertVariable("testVar5");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto q = PQL::Query({varDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testVar1", "testVar2", "testVar3", "testVar4", "testVar5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("no variable") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with no variable." << std::endl;
	  std::cout << "Testing query: variable v; Select v;" << std::endl;

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto q = PQL::Query({varDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("one procedure only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one procedure." << std::endl;
	  std::cout << "Testing query: procedure p; Select p;" << std::endl;

	  writer->insertProcedure("testProc1");

	  auto procDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p");
	  auto q = PQL::Query({procDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testProc1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple procedures") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple procedure." << std::endl;
	  std::cout << "Testing query: procedure p; Select p;" << std::endl;

	  writer->insertProcedure("testProc1");
	  writer->insertProcedure("testProc2");
	  writer->insertProcedure("testProc3");
	  writer->insertProcedure("testProc4");
	  writer->insertProcedure("testProc5");

	  auto procDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p");
	  auto q = PQL::Query({procDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testProc1", "testProc2", "testProc3", "testProc4", "testProc5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("one statement only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one statement." << std::endl;
	  std::cout << "Testing query: stmt s; Select s;" << std::endl;

	  writer->insertStatement(1);

	  auto stmtDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
	  auto q = PQL::Query({stmtDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple statements." << std::endl;
	  std::cout << "Testing query: stmt s; Select s;" << std::endl;

	  writer->insertStatement(1);
	  writer->insertStatement(2);
	  writer->insertStatement(3);
	  writer->insertStatement(4);
	  writer->insertStatement(5);

	  auto stmtDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
	  auto q = PQL::Query({stmtDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("one read only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one read." << std::endl;
	  std::cout << "Testing query: read r; Select r;" << std::endl;

	  writer->insertRead(1);

	  auto readDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
	  auto q = PQL::Query({readDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple reads") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple reads." << std::endl;
	  std::cout << "Testing query: read r; Select r;" << std::endl;

	  writer->insertRead(1);
	  writer->insertRead(2);
	  writer->insertRead(3);
	  writer->insertRead(4);
	  writer->insertRead(5);

	  auto readDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
	  auto q = PQL::Query({readDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("no read") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with no read." << std::endl;
	  std::cout << "Testing query: read r; Select r;" << std::endl;

	  auto readDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
	  auto q = PQL::Query({readDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("one print only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one print." << std::endl;
	  std::cout << "Testing query: print pn; Select pn;" << std::endl;

	  writer->insertPrint(1);

	  auto printDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
	  auto q = PQL::Query({printDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple prints") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple prints." << std::endl;
	  std::cout << "Testing query: print pn; Select pn;" << std::endl;

	  writer->insertPrint(1);
	  writer->insertPrint(2);
	  writer->insertPrint(3);
	  writer->insertPrint(4);
	  writer->insertPrint(5);

	  auto printDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
	  auto q = PQL::Query({printDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("no print") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with no print." << std::endl;
	  std::cout << "Testing query: print pn; Select pn;" << std::endl;

	  auto printDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
	  auto q = PQL::Query({printDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("one assign only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one assign." << std::endl;
	  std::cout << "Testing query: assign a; Select a;" << std::endl;

	  writer->insertAssign(1);

	  auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
	  auto q = PQL::Query({assignDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple assigns") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple assigns." << std::endl;
	  std::cout << "Testing query: assign a; Select a;" << std::endl;

	  writer->insertAssign(1);
	  writer->insertAssign(2);
	  writer->insertAssign(3);
	  writer->insertAssign(4);
	  writer->insertAssign(5);

	  auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
	  auto q = PQL::Query({assignDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("no assign") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with no assign." << std::endl;
	  std::cout << "Testing query: assign a; Select a;" << std::endl;

	  auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
	  auto q = PQL::Query({assignDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("one call only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one call." << std::endl;
	  std::cout << "Testing query: call c; Select c;" << std::endl;

	  writer->insertCall(1);

	  auto callDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
	  auto q = PQL::Query({callDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple calls") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple calls." << std::endl;
	  std::cout << "Testing query: call c; Select c;" << std::endl;

	  writer->insertCall(1);
	  writer->insertCall(2);
	  writer->insertCall(3);
	  writer->insertCall(4);
	  writer->insertCall(5);

	  auto callDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
	  auto q = PQL::Query({callDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("no call") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with no call." << std::endl;
	  std::cout << "Testing query: call c; Select c;" << std::endl;

	  auto callDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
	  auto q = PQL::Query({callDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("one while only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one while." << std::endl;
	  std::cout << "Testing query: while w; Select w;" << std::endl;

	  writer->insertWhile(1);

	  auto whileDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  auto q = PQL::Query({whileDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple whiles") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple whiles." << std::endl;
	  std::cout << "Testing query: while w; Select w;" << std::endl;

	  writer->insertWhile(1);
	  writer->insertWhile(2);
	  writer->insertWhile(3);
	  writer->insertWhile(4);
	  writer->insertWhile(5);

	  auto whileDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  auto q = PQL::Query({whileDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("no while") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with no while." << std::endl;
	  std::cout << "Testing query: while w; Select w;" << std::endl;

	  auto whileDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  auto q = PQL::Query({whileDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("one if only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one if." << std::endl;
	  std::cout << "Testing query: if ifs; Select ifs;" << std::endl;

	  writer->insertIf(1);

	  auto ifDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  auto q = PQL::Query({ifDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple ifs") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple ifs." << std::endl;
	  std::cout << "Testing query: if ifs; Select ifs;" << std::endl;

	  writer->insertIf(1);
	  writer->insertIf(2);
	  writer->insertIf(3);
	  writer->insertIf(4);
	  writer->insertIf(5);

	  auto ifDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  auto q = PQL::Query({ifDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("no if") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with no if." << std::endl;
	  std::cout << "Testing query: if ifs; Select ifs;" << std::endl;

	  auto ifDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  auto q = PQL::Query({ifDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("one constant only") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with only one constant." << std::endl;
	  std::cout << "Testing query: constant c; Select c;" << std::endl;

	  writer->insertConstant(1);

	  auto constDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
	  auto q = PQL::Query({constDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("multiple constants") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with multiple constants." << std::endl;
	  std::cout << "Testing query: constant c; Select c;" << std::endl;

	  writer->insertConstant(1);
	  writer->insertConstant(2);
	  writer->insertConstant(3);
	  writer->insertConstant(4);
	  writer->insertConstant(5);

	  auto constDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
	  auto q = PQL::Query({constDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("no constants") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program with no constant." << std::endl;
	  std::cout << "Testing query: constant c; Select c;" << std::endl;

	  auto constDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
	  auto q = PQL::Query({constDeclaration}, {}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}
  }

  SECTION("Evaluate select BOOLEAN queries") {
	SECTION("Return TRUE") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program for select BOOLEAN with TRUE result." << std::endl;
	  std::cout << "Testing query: Select BOOLEAN; such that Follows(1,2)" << std::endl;

	  writer->insertStatement({1, 2});
	  writer->insertFollows(1, 2);

	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::BOOLEAN, "BOOLEAN");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "1");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({}, {followClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"TRUE"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("Return FALSE") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program for select BOOLEAN with FALSE result." << std::endl;
	  std::cout << "Testing query: Select BOOLEAN; such that Follows(2,1)" << std::endl;

	  writer->insertStatement({1, 2});
	  writer->insertFollows(1, 2);

	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::BOOLEAN, "BOOLEAN");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "1");
	  auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({}, {followClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"FALSE"};
	  REQUIRE(res == expectedRes);
	}
  }

  SECTION("Evaluate select TUPLE queries") {
	SECTION("Non-empty results") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program for select TUPLE with empty result." << std::endl;
	  std::cout << "Testing query: stmt s1, s2; Select <s1, s2> such that Follows(s1, s2)" << std::endl;

	  writer->insertStatement({1, 2, 3, 4, 5});
	  writer->insertFollows(1, 2);
	  writer->insertFollows(1, 3);
	  writer->insertFollows(2, 3);
	  writer->insertFollows(3, 5);
	  writer->insertFollows(4, 5);

	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s1");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s2");
	  auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({}, {followClause}, {lArgSyn, rArgSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1 2", "1 3", "2 3", "3 5", "4 5"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("Empty results") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program for select TUPLE with empty result." << std::endl;
	  std::cout << "Testing query: stmt s1, s2; Select <s1, s2> such that Follows(s1, s2)" << std::endl;

	  writer->insertStatement({1, 2, 3, 4, 5});

	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s1");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s2");
	  auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({}, {followClause}, {lArgSyn, rArgSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("Non-empty results with more than 2 elements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing SIMPLE program for select TUPLE with empty result." << std::endl;
	  std::cout << "Testing query: variable v; stmt s1, s2; Select <s1, s2, v> such that Follows(s1, s2);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertStatement({1, 2, 3, 4, 5});
	  writer->insertFollows(1, 2);
	  writer->insertFollows(1, 3);
	  writer->insertFollows(2, 3);
	  writer->insertFollows(3, 5);
	  writer->insertFollows(4, 5);

	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s1");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s2");
	  auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varSyn}, {followClause}, {lArgSyn, rArgSyn, varSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"1 2 testVar1", "1 3 testVar1", "2 3 testVar1", "3 5 testVar1", "4 5 testVar1", "1 2 testVar2", "1 3 testVar2", "2 3 testVar2", "3 5 testVar2", "4 5 testVar2"};
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}
  }
}

