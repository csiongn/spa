#include "catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("Assign Pattern Evaluator") {
    SECTION("Evaluate entity evaluator with IDENT as left argument") {
        SECTION("WILDCARD right argument non-empty and correct statements") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with non-empty result that has lArg: IDENT, rArg: WILDCARD" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(TEST_IDENT, _);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2"};
            REQUIRE(res == expectedRes);
        }

        SECTION("WILDCARD right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: IDENT, rArg: WILDCARD" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(TEST_IDENT, _);" << std::endl;

            writer->insertVariable("testVar2");
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("EXPR right argument non-empty and correct statements") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: IDENT, rArg: EXPR" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(TEST_IDENT, \"x\");" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "x");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2"};
            REQUIRE(res == expectedRes);
        }

        SECTION("EXPR right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: IDENT, rArg: EXPR" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(TEST_IDENT, \"x\");" << std::endl;

            writer->insertVariable("testVar2");
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "_x_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(res == expectedRes);
        }

        SECTION("PARTIAL_EXPR right argument non-empty and correct statements") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: IDENT, rArg: PARTIAL_EXPR" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(TEST_IDENT, _ \" x \" _  );" << std::endl;

            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
            std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
            std::shared_ptr<ExprNode> twoPlusX = std::make_shared<BinaryExprNode>(two, "+", x);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            std::shared_ptr<ExprNode> yPlusTwo = std::make_shared<BinaryExprNode>(y, "+", two);

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertVariable("testVar3");
            writer->insertAssign(2);
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            writer->insertAssignPattern("testVar1", xPlusTwo->getHashValue(), 3, xPlusTwo);
            writer->insertAssign(4);
            writer->insertAssignPattern("testVar1", twoPlusX->getHashValue(), 4, twoPlusX);
            writer->insertAssign(5);
            writer->insertAssignPattern("testVar2", x->getHashValue(), 5, x);
            writer->insertAssign(6);
            writer->insertAssignPattern("testVar2", xPlusTwo->getHashValue(), 6, xPlusTwo);
            writer->insertAssign(7);
            writer->insertAssignPattern("testVar3", y->getHashValue(), 7, y);
            writer->insertAssign(8);
            writer->insertAssignPattern("testVar3", yPlusTwo->getHashValue(), 7, yPlusTwo);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_x_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2", "3", "4"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("PARTIAL_EXPR right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: IDENT, rArg: PARTIAL_EXPR" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(TEST_IDENT, _ \" y \" _  );" << std::endl;

            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
            std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
            std::shared_ptr<ExprNode> twoPlusX = std::make_shared<BinaryExprNode>(two, "+", x);

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            writer->insertAssignPattern("testVar1", xPlusTwo->getHashValue(), 3, xPlusTwo);
            writer->insertAssign(4);
            writer->insertAssignPattern("testVar1", twoPlusX->getHashValue(), 4, twoPlusX);
            writer->insertAssign(5);
            writer->insertAssignPattern("testVar2", x->getHashValue(), 5, x);
            writer->insertAssign(6);
            writer->insertAssignPattern("testVar2", xPlusTwo->getHashValue(), 6, xPlusTwo);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_y_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }
    }

    SECTION("Evaluate entity evaluator with VAR as left argument") {
        SECTION("WILDCARD right argument non-empty and correct statements") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: WILDCARD" << std::endl;
            std::cout << "Testing query variable v; assign a; Select a such that a(v, _);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2", "3"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("WILDCARD right argument non-empty and correct variable name") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: WILDCARD" << std::endl;
            std::cout << "Testing query variable v; assign a; Select v such that a(v, _);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"testVar1", "testVar2"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("WILDCARD right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: WILDCARD" << std::endl;
            std::cout << "Testing query variable v; assign a; Select v such that a(v, _);" << std::endl;

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("EXPR right argument non-empty and correct variable name") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: EXPR" << std::endl;
            std::cout << "Testing query variable v; assign a; Select v such that a(v, x);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "x");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"testVar1"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("EXPR right argument non-empty and correct statement") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: EXPR" << std::endl;
            std::cout << "Testing query variable v; assign a; Select v such that a(v, x);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);
            writer->insertAssign(5);
            writer->insertAssignPattern("testVar1", x->getHashValue(), 5, x);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "x");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2", "5"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("EXPR right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: EXPR" << std::endl;
            std::cout << "Testing query variable v; assign a; Select v such that a(v, x);" << std::endl;

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "x");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("PARTIAL_EXPR right argument non-empty and correct statements") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: PARTIAL_EXPR" << std::endl;
            std::cout << "Testing query variable v; assign a; Select a such that a(v, _ \" x \" _  );" << std::endl;

            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
            std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
            std::shared_ptr<ExprNode> twoPlusX = std::make_shared<BinaryExprNode>(two, "+", x);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            std::shared_ptr<ExprNode> yPlusTwo = std::make_shared<BinaryExprNode>(y, "+", two);

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertVariable("testVar3");
            writer->insertAssign(2);
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            writer->insertAssignPattern("testVar1", xPlusTwo->getHashValue(), 3, xPlusTwo);
            writer->insertAssign(4);
            writer->insertAssignPattern("testVar1", twoPlusX->getHashValue(), 4, twoPlusX);
            writer->insertAssign(5);
            writer->insertAssignPattern("testVar2", x->getHashValue(), 5, x);
            writer->insertAssign(6);
            writer->insertAssignPattern("testVar2", xPlusTwo->getHashValue(), 6, xPlusTwo);
            writer->insertAssign(7);
            writer->insertAssignPattern("testVar3", y->getHashValue(), 7, y);
            writer->insertAssign(8);
            writer->insertAssignPattern("testVar3", yPlusTwo->getHashValue(), 7, yPlusTwo);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_x_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2", "3", "4", "5", "6"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("PARTIAL_EXPR right argument non-empty and correct variable") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: PARTIAL_EXPR" << std::endl;
            std::cout << "Testing query variable v; assign a; Select a such that a(v, _ \" x \" _  );" << std::endl;

            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
            std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
            std::shared_ptr<ExprNode> twoPlusX = std::make_shared<BinaryExprNode>(two, "+", x);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            std::shared_ptr<ExprNode> yPlusTwo = std::make_shared<BinaryExprNode>(y, "+", two);

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertVariable("testVar3");
            writer->insertAssign(2);
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            writer->insertAssignPattern("testVar1", xPlusTwo->getHashValue(), 3, xPlusTwo);
            writer->insertAssign(4);
            writer->insertAssignPattern("testVar1", twoPlusX->getHashValue(), 4, twoPlusX);
            writer->insertAssign(5);
            writer->insertAssignPattern("testVar2", x->getHashValue(), 5, x);
            writer->insertAssign(6);
            writer->insertAssignPattern("testVar2", xPlusTwo->getHashValue(), 6, xPlusTwo);
            writer->insertAssign(7);
            writer->insertAssignPattern("testVar3", y->getHashValue(), 7, y);
            writer->insertAssign(8);
            writer->insertAssignPattern("testVar3", yPlusTwo->getHashValue(), 7, yPlusTwo);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_x_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"testVar1", "testVar2"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("PARTIAL_EXPR right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: PARTIAL_EXPR" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(v, _ \" y \" _  );" << std::endl;

            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
            std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
            std::shared_ptr<ExprNode> twoPlusX = std::make_shared<BinaryExprNode>(two, "+", x);

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            writer->insertAssignPattern("testVar1", xPlusTwo->getHashValue(), 3, xPlusTwo);
            writer->insertAssign(4);
            writer->insertAssignPattern("testVar1", twoPlusX->getHashValue(), 4, twoPlusX);
            writer->insertAssign(5);
            writer->insertAssignPattern("testVar2", x->getHashValue(), 5, x);
            writer->insertAssign(6);
            writer->insertAssignPattern("testVar2", xPlusTwo->getHashValue(), 6, xPlusTwo);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_y_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }
    }

    SECTION("Evaluate entity evaluator with WILDCARD as left argument") {
        SECTION("WILDCARD right argument non-empty and correct statements") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: WILDCARD" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(_, _);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);
            writer->insertAssign(4);
            writer->insertAssign(5);
            writer->insertAssign(6);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2", "3", "4", "5", "6"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("WILDCARD right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: WILDCARD" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(_, _);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("EXPR right argument non-empty and correct statements") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: EXPR" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(_, x);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);
            writer->insertAssign(4);
            writer->insertAssign(5);
            writer->insertAssign(6);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "x");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("EXPR right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: EXPR" << std::endl;
            std::cout << "Testing query assign a; Select a such that a(_, x);" << std::endl;

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(3);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            writer->insertAssignPattern("testVar2", y->getHashValue(), 3, y);
            writer->insertAssign(4);
            writer->insertAssign(5);
            writer->insertAssign(6);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "x");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("PARTIAL_EXPR right argument non-empty and correct statements") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: PARTIAL_EXPR" << std::endl;
            std::cout << "Testing query variable v; assign a; Select a such that a(_, _ \" x \" _  );" << std::endl;

            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
            std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
            std::shared_ptr<ExprNode> twoPlusX = std::make_shared<BinaryExprNode>(two, "+", x);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            std::shared_ptr<ExprNode> yPlusTwo = std::make_shared<BinaryExprNode>(y, "+", two);


            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            writer->insertAssignPattern("testVar1", xPlusTwo->getHashValue(), 3, xPlusTwo);
            writer->insertAssign(4);
            writer->insertAssignPattern("testVar1", twoPlusX->getHashValue(), 4, twoPlusX);
            writer->insertAssign(5);
            writer->insertAssignPattern("testVar2", x->getHashValue(), 5, x);
            writer->insertAssign(6);
            writer->insertAssignPattern("testVar2", xPlusTwo->getHashValue(), 6, xPlusTwo);
            writer->insertAssign(7);
            writer->insertAssignPattern("testVar2", y->getHashValue(), 7, y);
            writer->insertAssign(8);
            writer->insertAssignPattern("testVar2", yPlusTwo->getHashValue(), 7, yPlusTwo);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_x_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {"2", "3", "4", "5", "6"};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }

        SECTION("PARTIAL_EXPR right argument empty") {
            auto pkb = std::make_shared<PKB>();
            std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
            std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
            QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

            std::cout << "============ Start testing ============" << std::endl;
            std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: PARTIAL_EXPR" << std::endl;
            std::cout << "Testing query variable v; assign a; Select a such that a(_, _ \" x \" _  );" << std::endl;

            std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
            std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
            std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
            std::shared_ptr<ExprNode> twoPlusX = std::make_shared<BinaryExprNode>(two, "+", x);
            std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
            std::shared_ptr<ExprNode> yPlusTwo = std::make_shared<BinaryExprNode>(y, "+", two);

            writer->insertVariable("testVar1");
            writer->insertVariable("testVar2");
            writer->insertAssign(2);
            writer->insertAssignPattern("testVar1", x->getHashValue(), 2, x);
            writer->insertAssign(3);
            writer->insertAssignPattern("testVar1", xPlusTwo->getHashValue(), 3, xPlusTwo);
            writer->insertAssign(4);
            writer->insertAssignPattern("testVar1", twoPlusX->getHashValue(), 4, twoPlusX);
            writer->insertAssign(5);
            writer->insertAssignPattern("testVar2", x->getHashValue(), 5, x);
            writer->insertAssign(6);
            writer->insertAssignPattern("testVar2", xPlusTwo->getHashValue(), 6, xPlusTwo);
            writer->insertAssign(7);
            writer->insertAssignPattern("testVar2", y->getHashValue(), 7, y);
            writer->insertAssign(8);
            writer->insertAssignPattern("testVar2", yPlusTwo->getHashValue(), 7, yPlusTwo);

            auto assignDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
            auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a");
            auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
            auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_z_");

            PQL::Clause assignPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, {assignDeclaration, lArgSyn, rArgSyn});
            PQL::Query q = PQL::Query({assignDeclaration, varDeclaration}, {assignPatternClause}, selectSyn);
            std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
            std::vector<std::string> expectedRes = {};
            REQUIRE(checkVecValuesEqual(res, expectedRes));
        }
    }
}