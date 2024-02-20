#include "catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("Evaluate query with only select") {

    auto pkb = std::make_shared<PKB>();
    std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
    std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;

    QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

    SECTION("one variable only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one variable." << std::endl;
        std::cout << "Testing query: variable v; Select v;" << std::endl;

        writer->insertVariable("testVar1");

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto q = PQL::Query({varDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testVar1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple variables") {
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
        auto q = PQL::Query({varDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testVar1", "testVar2", "testVar3","testVar4", "testVar5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no variable") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no variable." << std::endl;
        std::cout << "Testing query: variable v; Select v;" << std::endl;

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto q = PQL::Query({varDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("one procedure only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one procedure." << std::endl;
        std::cout << "Testing query: procedure p; Select p;" << std::endl;

        writer->insertProcedure("testProc1");

        auto procDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::PROCEDURE, "p");
        auto q = PQL::Query({procDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testProc1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple procedures") {
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
        auto q = PQL::Query({procDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testProc1", "testProc2", "testProc3", "testProc4", "testProc5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("one statement only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one statement." << std::endl;
        std::cout << "Testing query: stmt s; Select s;" << std::endl;

        writer->insertStatement(1);

        auto stmtDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT, "s");
        auto q = PQL::Query({stmtDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple statements") {
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
        auto q = PQL::Query({stmtDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("one read only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one read." << std::endl;
        std::cout << "Testing query: read r; Select r;" << std::endl;

        writer->insertRead(1);

        auto readDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
        auto q = PQL::Query({readDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple reads") {
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
        auto q = PQL::Query({readDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no read") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no read." << std::endl;
        std::cout << "Testing query: read r; Select r;" << std::endl;

        auto readDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::READ, "r");
        auto q = PQL::Query({readDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("one print only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one print." << std::endl;
        std::cout << "Testing query: print pn; Select pn;" << std::endl;

        writer->insertPrint(1);

        auto printDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
        auto q = PQL::Query({printDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple prints") {
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
        auto q = PQL::Query({printDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no print") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no print." << std::endl;
        std::cout << "Testing query: print pn; Select pn;" << std::endl;

        auto printDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "pn");
        auto q = PQL::Query({printDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("one assign only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one assign." << std::endl;
        std::cout << "Testing query: assign a; Select a;" << std::endl;

        writer->insertAssign(1);

        auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
        auto q = PQL::Query({assignDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple assigns") {
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
        auto q = PQL::Query({assignDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no assign") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no assign." << std::endl;
        std::cout << "Testing query: assign a; Select a;" << std::endl;

        auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
        auto q = PQL::Query({assignDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("one call only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one call." << std::endl;
        std::cout << "Testing query: call c; Select c;" << std::endl;

        writer->insertCall(1);

        auto callDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
        auto q = PQL::Query({callDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple calls") {
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
        auto q = PQL::Query({callDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no call") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no call." << std::endl;
        std::cout << "Testing query: call c; Select c;" << std::endl;

        auto callDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CALL, "c");
        auto q = PQL::Query({callDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("one while only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one while." << std::endl;
        std::cout << "Testing query: while w; Select w;" << std::endl;

        writer->insertWhile(1);

        auto whileDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
        auto q = PQL::Query({whileDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple whiles") {
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
        auto q = PQL::Query({whileDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no while") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no while." << std::endl;
        std::cout << "Testing query: while w; Select w;" << std::endl;

        auto whileDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
        auto q = PQL::Query({whileDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("one if only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one if." << std::endl;
        std::cout << "Testing query: if ifs; Select ifs;" << std::endl;

        writer->insertIf(1);

        auto ifDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
        auto q = PQL::Query({ifDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple ifs") {
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
        auto q = PQL::Query({ifDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no if") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no if." << std::endl;
        std::cout << "Testing query: if ifs; Select ifs;" << std::endl;

        auto ifDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
        auto q = PQL::Query({ifDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("one constant only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with only one constant." << std::endl;
        std::cout << "Testing query: constant c; Select c;" << std::endl;

        writer->insertConstant(1);

        auto constDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
        auto q = PQL::Query({constDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple constants") {
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
        auto q = PQL::Query({constDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"1", "2", "3", "4", "5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no constants") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no constant." << std::endl;
        std::cout << "Testing query: constant c; Select c;" << std::endl;

        auto constDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c");
        auto q = PQL::Query({constDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }
}

TEST_CASE("Evaluate statement only queries with two statement number arguments") {

    auto pkb = std::make_shared<PKB>();
    std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
    std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;

    QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

    SECTION("follows and valid") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing PQL query with valid follows that has both statement number as arguments." << std::endl;
        std::cout << "Testing query: variable v; Select v such that Follow(2,3);" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertFollows(2,3);

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
        auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
        auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
        auto q = PQL::Query({varDeclaration}, {followClause}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testVar1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("follows and invalid") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing PQL query with invalid follows that has both statement number as arguments." << std::endl;
        std::cout << "Testing query: variable v; Select v such that Follow(1,3);" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertFollows(2,3);

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "1");
        auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
        auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
        auto q = PQL::Query({varDeclaration}, {followClause}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("followsT and valid") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing PQL query with valid follows* that has both statement number as arguments." << std::endl;
        std::cout << "Testing query: variable v; Select v such that Follow*(2,5);" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertFollowsT(2,5);

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
        auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "5");
        auto followTClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn});
        auto q = PQL::Query({varDeclaration}, {followTClause}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testVar1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("followsT and invalid") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing PQL query with invalid follows* that has both statement number as arguments." << std::endl;
        std::cout << "Testing query: variable v; Select v such that Follow*(5,2);" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertFollowsT(2,5);

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "5");
        auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
        auto followTClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn});
        auto q = PQL::Query({varDeclaration}, {followTClause}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("parent and valid") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing PQL query with valid parent that has both statement number as arguments." << std::endl;
        std::cout << "Testing query: variable v; Select v such that Parent(2,3);" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertParent(2,3);

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
        auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
        auto parentClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn});
        auto q = PQL::Query({varDeclaration}, {parentClause}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testVar1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("parent and invalid") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing PQL query with invalid parent that has both statement number as arguments." << std::endl;
        std::cout << "Testing query: variable v; Select v such that Parent(3,2);" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertParent(2,3);

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "1");
        auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
        auto parentClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn});
        auto q = PQL::Query({varDeclaration}, {parentClause}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }

    SECTION("parentT and valid") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing PQL query with valid parent* that has both statement number as arguments." << std::endl;
        std::cout << "Testing query: variable v; Select v such that Parent*(2,5);" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertParentT(2,5);

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
        auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "5");
        auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn});
        auto q = PQL::Query({varDeclaration}, {parentTClause}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testVar1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("parentT and invalid") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing PQL query with invalid parent* that has both statement number as arguments." << std::endl;
        std::cout << "Testing query: variable v; Select v such that Parent*(5,2);" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertParentT(2,5);

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "5");
        auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
        auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn});
        auto q = PQL::Query({varDeclaration}, {parentTClause}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }
}



