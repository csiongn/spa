#include "catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("EVALUATOR TEST") {

    SECTION("Evaluate query with only select") {
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
            std::vector<std::string> expectedRes = {"testVar1", "testVar2", "testVar3", "testVar4", "testVar5"};
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

    SECTION("Evaluate statement only queries with two statement number arguments") {

        auto pkb = std::make_shared<PKB>();
        std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
        std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;

        QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

        SECTION("follows and non-empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with follows that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Follow(2,3);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertFollows(2, 3);

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

        SECTION("follows and empty (same statement number)") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with follows that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Follow(2,2);" << std::endl;

            writer->insertVariable("testVar1");
            // not supposed to have such relationship, added just to test
            writer->insertFollowsT(2, 2);

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
            auto q = PQL::Query({varDeclaration}, {followClause}, selectSyn);

            auto res = evaluator.evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("follows and empty (different statement number)") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with follows that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Follow(2,3);" << std::endl;

            writer->insertVariable("testVar1");

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
            auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
            auto q = PQL::Query({varDeclaration}, {followClause}, selectSyn);

            auto res = evaluator.evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("followsT and non-empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with follows* that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Follow*(2,5);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertFollowsT(2, 5);

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

        SECTION("followsT and empty (same statement number)") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with follows* that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Follow*(2,2);" << std::endl;

            writer->insertVariable("testVar1");
            // not supposed to have such relationship, added just to test
            writer->insertFollowsT(2, 2);

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto followTClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn});
            auto q = PQL::Query({varDeclaration}, {followTClause}, selectSyn);

            auto res = evaluator.evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("followsT and empty (different statement number)") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with follows* that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Follow*(2,3);" << std::endl;

            writer->insertVariable("testVar1");

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
            auto followTClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn});
            auto q = PQL::Query({varDeclaration}, {followTClause}, selectSyn);

            auto res = evaluator.evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("parent and non-empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with parent that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Parent(2,3);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertParent(2, 3);

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

        SECTION("parent and empty (same statement number") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with parent that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Parent(2,2);" << std::endl;

            writer->insertVariable("testVar1");
            // not supposed to have such relationship, added just to test
            writer->insertFollowsT(2, 2);

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto parentClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn});
            auto q = PQL::Query({varDeclaration}, {parentClause}, selectSyn);

            auto res = evaluator.evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("parent and empty (different statement number") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with parent that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Parent(2,3);" << std::endl;

            writer->insertVariable("testVar1");

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
            auto parentClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn});
            auto q = PQL::Query({varDeclaration}, {parentClause}, selectSyn);

            auto res = evaluator.evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("parentT and non-empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Parent*(2,5);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertParentT(2, 5);

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

        SECTION("parentT and empty (same statement number)") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Parent*(2,2);" << std::endl;

            writer->insertVariable("testVar1");
            // not supposed to have such relationship, added just to test
            writer->insertFollowsT(2, 2);

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn});
            auto q = PQL::Query({varDeclaration}, {parentTClause}, selectSyn);

            auto res = evaluator.evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("parentT and empty (different statement number)") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
            std::cout << "Testing query: variable v; Select v such that Parent*(2,3);" << std::endl;

            writer->insertVariable("testVar1");

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
            auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn});
            auto q = PQL::Query({varDeclaration}, {parentTClause}, selectSyn);

            auto res = evaluator.evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }
    }

    SECTION("Evaluate statement only queries with STMT_NUM as left argument") {

        auto pkb = std::make_shared<PKB>();
        std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
        std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;

        QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

        SECTION("WILDCARD non-empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with non-empty result that has lArg: STMT_NUM, rArg: WILDCARD."
                      << std::endl;
            std::cout << "Testing query: variable v; Select v such that XXX(2, _);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertFollows(2, 3);
            writer->insertFollowsT(2, 3);
            writer->insertParent(2, 3);
            writer->insertParentT(2, 3);

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            std::vector<PQL::Clause> clauses = {
                    PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn})
            };

            // no default constructor
            PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, selectSyn);
            std::vector<std::string> res;
            std::vector<std::string> expectedRes = {"testVar1"};
            for (auto const &cl: clauses) {
                q = PQL::Query({varDeclaration}, {cl}, selectSyn);
                res = evaluator.evaluateQuery(q);
                REQUIRE(res == expectedRes);
                switch (cl.clauseType) {
                    case SimpleProgram::DesignAbstraction::FOLLOWS:
                        std::cout << "FOLLOWS passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::FOLLOWST:
                        std::cout << "FOLLOWST passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::PARENT:
                        std::cout << "PARENT passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::PARENTT:
                        std::cout << "PARENTT passed" << std::endl;
                        break;
                    default:
                        std::cout << "UNKNOWN CLAUSE" << std::endl;
                }
            }
        }

        SECTION("WILDCARD empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: WILDCARD." << std::endl;
            std::cout << "Testing query: variable v; Select v such that XXX(2, _);" << std::endl;

            writer->insertVariable("testVar1");

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            std::vector<PQL::Clause> clauses = {
                    PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn})
            };

            // no default constructor
            PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, selectSyn);
            std::vector<std::string> res;
            std::vector<std::string> expectedRes = {};
            for (auto const &cl: clauses) {
                q = PQL::Query({varDeclaration}, {cl}, selectSyn);
                res = evaluator.evaluateQuery(q);
                REQUIRE(res == expectedRes);
                switch (cl.clauseType) {
                    case SimpleProgram::DesignAbstraction::FOLLOWS:
                        std::cout << "FOLLOWS passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::FOLLOWST:
                        std::cout << "FOLLOWST passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::PARENT:
                        std::cout << "PARENT passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::PARENTT:
                        std::cout << "PARENTT passed" << std::endl;
                        break;
                    default:
                        std::cout << "UNKNOWN CLAUSE" << std::endl;
                }
            }
        }

        SECTION("SYNONYM non-empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with non-empty result that has lArg: STMT_NUM, rArg: SYN." << std::endl;
            std::cout << "Testing query: variable v; Select v such that XXX(2, SYN);" << std::endl;


            writer->insertVariable("testVar1");
            writer->insertFollows(2, 3);
            writer->insertFollowsT(2, 3);
            writer->insertParent(2, 3);
            writer->insertParentT(2, 3);
            writer->insertStatement(3);
            writer->insertRead(3);
            writer->insertPrint(3);
            writer->insertAssign(3);
            writer->insertCall(3);
            writer->insertWhile(3);
            writer->insertIf(3);

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            std::vector<PQL::Synonym> declarations = {
                    PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
                    PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
                    PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
                    PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
                    PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
                    PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
                    PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
            };

            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

            // no default constructor
            std::vector<PQL::Clause> clauses = {PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, lArgSyn}}};

            // no default constructor
            PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, selectSyn);
            std::vector<std::string> res;
            std::vector<std::string> expectedRes = {"testVar1"};
            for (auto const &syn: declarations) {
                clauses = {
                        PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, syn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, syn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, syn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, syn})
                };

                for (auto const &cl: clauses) {
                    switch (cl.clauseType) {
                        case SimpleProgram::DesignAbstraction::FOLLOWS:
                            std::cout << "TESTING FOLLOWS" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::FOLLOWST:
                            std::cout << "TESTING FOLLOWST" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::PARENT:
                            std::cout << "TESTING PARENT" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::PARENTT:
                            std::cout << "TESTING PARENTT" << std::endl;
                            break;
                        default:
                            std::cout << "UNKNOWN CLAUSE" << std::endl;
                    }
                    q = PQL::Query({varDeclaration, syn}, {cl}, selectSyn);
                    res = evaluator.evaluateQuery(q);
                    REQUIRE(res == expectedRes);
                    std::cout << syn.identity << " passed" << std::endl;
                }
            }
        }

        SECTION("SYNONYM empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: SYN." << std::endl;
            std::cout << "Testing query: variable v; Select v such that XXX(2, SYN);" << std::endl;

            writer->insertVariable("testVar1");

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            std::vector<PQL::Synonym> declarations = {
                    PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
                    PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
                    PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
                    PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
                    PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
                    PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
                    PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
            };

            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

            // no default constructor
            std::vector<PQL::Clause> clauses = {PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, lArgSyn}}};

            // no default constructor
            PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, selectSyn);
            std::vector<std::string> res;
            std::vector<std::string> expectedRes = {};
            for (auto const &syn: declarations) {
                clauses = {
                        PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, syn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, syn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, syn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, syn})
                };

                for (auto const &cl: clauses) {
                    switch (cl.clauseType) {
                        case SimpleProgram::DesignAbstraction::FOLLOWS:
                            std::cout << "TESTING FOLLOWS" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::FOLLOWST:
                            std::cout << "TESTING FOLLOWST" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::PARENT:
                            std::cout << "TESTING PARENT" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::PARENTT:
                            std::cout << "TESTING PARENTT" << std::endl;
                            break;
                        default:
                            std::cout << "UNKNOWN CLAUSE" << std::endl;
                    }
                    q = PQL::Query({varDeclaration, syn}, {cl}, selectSyn);
                    res = evaluator.evaluateQuery(q);
                    REQUIRE(res == expectedRes);
                    std::cout << syn.identity << " passed" << std::endl;
                }
            }
        }
    }

    SECTION("Evaluate statement only queries with STMT_NUM as right argument") {

        auto pkb = std::make_shared<PKB>();
        std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
        std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;

        QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

        SECTION("WILDCARD non-empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with non-empty result that has lArg: WILDCARD, rArg: STMT_NUM."
                      << std::endl;
            std::cout << "Testing query: variable v; Select v such that XXX(_, 2);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertFollows(1, 2);
            writer->insertFollowsT(1, 2);
            writer->insertParent(1, 2);
            writer->insertParentT(1, 2);

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

            std::vector<PQL::Clause> clauses = {
                    PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn})
            };

            // no default constructor
            PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, selectSyn);
            std::vector<std::string> res;
            std::vector<std::string> expectedRes = {"testVar1"};
            for (auto const &cl: clauses) {
                q = PQL::Query({varDeclaration}, {cl}, selectSyn);
                res = evaluator.evaluateQuery(q);
                REQUIRE(res == expectedRes);
                switch (cl.clauseType) {
                    case SimpleProgram::DesignAbstraction::FOLLOWS:
                        std::cout << "FOLLOWS passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::FOLLOWST:
                        std::cout << "FOLLOWST passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::PARENT:
                        std::cout << "PARENT passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::PARENTT:
                        std::cout << "PARENTT passed" << std::endl;
                        break;
                    default:
                        std::cout << "UNKNOWN CLAUSE" << std::endl;
                }
            }
        }

        SECTION("WILDCARD empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: STMT_NUM." << std::endl;
            std::cout << "Testing query: variable v; Select v such that XXX(_, 2);" << std::endl;

            writer->insertVariable("testVar1");

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

            std::vector<PQL::Clause> clauses = {
                    PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
                    PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn})
            };

            // no default constructor
            PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, selectSyn);
            std::vector<std::string> res;
            std::vector<std::string> expectedRes = {};
            for (auto const &cl: clauses) {
                q = PQL::Query({varDeclaration}, {cl}, selectSyn);
                res = evaluator.evaluateQuery(q);
                REQUIRE(res == expectedRes);
                switch (cl.clauseType) {
                    case SimpleProgram::DesignAbstraction::FOLLOWS:
                        std::cout << "FOLLOWS passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::FOLLOWST:
                        std::cout << "FOLLOWST passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::PARENT:
                        std::cout << "PARENT passed" << std::endl;
                        break;
                    case SimpleProgram::DesignAbstraction::PARENTT:
                        std::cout << "PARENTT passed" << std::endl;
                        break;
                    default:
                        std::cout << "UNKNOWN CLAUSE" << std::endl;
                }
            }
        }

        SECTION("SYNONYM non-empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: STMT_NUM." << std::endl;
            std::cout << "Testing query: variable v; Select v such that XXX(SYN, 2);" << std::endl;


            writer->insertVariable("testVar1");
            writer->insertFollows(1, 2);
            writer->insertFollowsT(1, 2);
            writer->insertParent(1, 2);
            writer->insertParentT(1, 2);
            writer->insertStatement(2);
            writer->insertRead(1);
            writer->insertPrint(1);
            writer->insertAssign(1);
            writer->insertCall(1);
            writer->insertWhile(1);
            writer->insertIf(1);

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            std::vector<PQL::Synonym> declarations = {
                    PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
                    PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
                    PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
                    PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
                    PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
                    PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
                    PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
            };

            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

            // no default constructor
            std::vector<PQL::Clause> clauses = {PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {rArgSyn, rArgSyn}}};

            // no default constructor
            PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, selectSyn);
            std::vector<std::string> res;
            std::vector<std::string> expectedRes = {"testVar1"};
            for (auto const &syn: declarations) {
                clauses = {
                        PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {syn, rArgSyn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {syn, rArgSyn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {syn, rArgSyn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {syn, rArgSyn})
                };

                for (auto const &cl: clauses) {
                    switch (cl.clauseType) {
                        case SimpleProgram::DesignAbstraction::FOLLOWS:
                            std::cout << "TESTING FOLLOWS" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::FOLLOWST:
                            std::cout << "TESTING FOLLOWST" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::PARENT:
                            std::cout << "TESTING PARENT" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::PARENTT:
                            std::cout << "TESTING PARENTT" << std::endl;
                            break;
                        default:
                            std::cout << "UNKNOWN CLAUSE" << std::endl;
                    }
                    q = PQL::Query({varDeclaration, syn}, {cl}, selectSyn);
                    res = evaluator.evaluateQuery(q);
                    REQUIRE(res == expectedRes);
                    std::cout << syn.identity << " passed" << std::endl;
                }
            }
        }

        SECTION("SYNONYM empty") {
            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: SYN, rArg: STMT_NUM." << std::endl;
            std::cout << "Testing query: variable v; Select v such that XXX(SYN, 2);" << std::endl;

            writer->insertVariable("testVar1");

            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            std::vector<PQL::Synonym> declarations = {
                    PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
                    PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
                    PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
                    PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
                    PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
                    PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
                    PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
            };

            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

            // no default constructor
            std::vector<PQL::Clause> clauses = {PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {rArgSyn, rArgSyn}}};

            // no default constructor
            PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, selectSyn);
            std::vector<std::string> res;
            std::vector<std::string> expectedRes = {};
            for (auto const &syn: declarations) {
                clauses = {
                        PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {syn, rArgSyn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {syn, rArgSyn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {syn, rArgSyn}),
                        PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {syn, rArgSyn})
                };

                for (auto const &cl: clauses) {
                    switch (cl.clauseType) {
                        case SimpleProgram::DesignAbstraction::FOLLOWS:
                            std::cout << "TESTING FOLLOWS" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::FOLLOWST:
                            std::cout << "TESTING FOLLOWST" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::PARENT:
                            std::cout << "TESTING PARENT" << std::endl;
                            break;
                        case SimpleProgram::DesignAbstraction::PARENTT:
                            std::cout << "TESTING PARENTT" << std::endl;
                            break;
                        default:
                            std::cout << "UNKNOWN CLAUSE" << std::endl;
                    }
                    q = PQL::Query({varDeclaration, syn}, {cl}, selectSyn);
                    res = evaluator.evaluateQuery(q);
                    REQUIRE(res == expectedRes);
                    std::cout << syn.identity << " passed" << std::endl;
                }
            }
        }
    }
}

