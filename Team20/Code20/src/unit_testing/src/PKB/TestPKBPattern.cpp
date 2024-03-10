#include "PKB/manager/pattern/PatternManager.h"
#include "catch.hpp"
#include "PKB/Database.h"
#include "../TestUtils.h"
#include "PKB/PKB.h"


TEST_CASE("PKB Pattern Test") {
    // initialise database
    auto db = std::make_shared<Database>();
    // Create Nodes
    uint16_t stmtNum = 5;
    uint16_t dupStmtNum = 8;
    std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
    std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
    std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
    std::shared_ptr<StmtNode> assignStmt = std::make_shared<AssignNode>(stmtNum, "x", xPlusTwo);
    // create duplicate of x and two
    std::shared_ptr<ExprNode> xDup = std::make_shared<VariableNode>("x");
    std::shared_ptr<ExprNode> twoDup = std::make_shared<LiteralNode>("2");
    std::shared_ptr<ExprNode> xPlusTwoDup = std::make_shared<BinaryExprNode>(xDup, "+", twoDup);
    auto dupHashValue = xPlusTwoDup->getHashValue();
    auto expectedHashValue = xPlusTwo->getHashValue();
    REQUIRE(dupHashValue==expectedHashValue);
    SECTION("AssignPatternManager") {
        // T, U need to be var : string and U is ExprNode's Hash size_t, V is ptr
        // LHS Variable Name, RHS Node Hash Value, Ptr to Node

        // Test direct assignPatternManager first
        auto assignPatternManager = db->getAssignPatternManager();
        // Insert
        assignPatternManager->insert("x", xPlusTwo->getHashValue(), stmtNum, xPlusTwo);
        // Insert duplicate
        assignPatternManager->insert("x", dupHashValue, dupStmtNum, xPlusTwoDup);
        // Get

        auto result = assignPatternManager->getRHS("x");
        REQUIRE(result == std::vector<size_t>{xPlusTwo->getHashValue()});
        auto result2 = assignPatternManager->getLHS(xPlusTwo->getHashValue());
        REQUIRE(result2 == std::vector<std::string>{"x"});

        // Test contains
        REQUIRE(assignPatternManager->containsPattern("x", xPlusTwo->getHashValue()));
        REQUIRE(assignPatternManager->containsPattern("x", dupHashValue));

        // test getAllRHSNodePtrs
        auto result3 = assignPatternManager->getRHSNodePtr();
        REQUIRE(checkVecValuesEqual(result3 ,std::vector<std::shared_ptr<ExprNode>>{xPlusTwo, xPlusTwoDup}));
    }

    SECTION("IfPatternManager") {
        auto pkb = std::make_shared<PKB>();
        auto pkbFacade = pkb->pkbFacade;

        bool hasIfPattern = pkbFacade->hasIfPattern();
        REQUIRE(hasIfPattern == false);

        // insertIfPattern

        // if (x == y)
        pkbFacade->insertIfPattern("x", 5);
        pkbFacade->insertIfPattern("y", 5);

        // if (x == 0)
        pkbFacade->insertIfPattern("x", 10);

        // insert Set
        std::unordered_set<int> stmtSet = {2, 3};
        pkbFacade->insertIfPattern("x", stmtSet);

        // getIfPatternStmtNum
        std::vector<int> stmtNums = pkbFacade->getIfPatternStmtNum("x");
        std::vector<int> expectedStmtNums = {5, 10, 2, 3};
        REQUIRE(checkVecValuesEqual(stmtNums, expectedStmtNums));

        // getIfPatternVariable
        std::vector<std::string> variables = pkbFacade->getIfPatternVariable(5);
        std::vector<std::string> expectedVariables = {"x", "y"};
        REQUIRE(checkVecValuesEqual(variables, expectedVariables));

        // hasIfPattern
        bool hasIfPatternPostInsertion = pkbFacade->hasIfPattern();
        REQUIRE(hasIfPatternPostInsertion == true);

        // containsIfPattern
        bool containsIfPattern = pkbFacade->containsIfPattern("x", 5);
        REQUIRE(containsIfPattern == true);

        // containsIfPatternStmt
        bool containsIfPatternStmt = pkbFacade->containsIfPatternStmt(5);
        REQUIRE(containsIfPatternStmt == true);

        // containsIfPatternVariable
        bool containsIfPatternVariable = pkbFacade->containsIfPatternVariable("x");
        REQUIRE(containsIfPatternVariable == true);
    }

    SECTION("WhilePatternManager") {
        auto pkb = std::make_shared<PKB>();
        auto pkbFacade = pkb->pkbFacade;

        bool hasWhilePattern = pkbFacade->hasWhilePattern();
        REQUIRE(hasWhilePattern == false);

        // insertWhilePattern

        // while (z == w)
        pkbFacade->insertWhilePattern("z", 7);
        pkbFacade->insertWhilePattern("w", 7);

        // while (z == 0)
        pkbFacade->insertWhilePattern("z", 11);

        pkbFacade->insertWhilePattern("g", 15);

        // insert Set
        std::unordered_set<int> stmtSet = {4,8};
        pkbFacade->insertWhilePattern("z", stmtSet);

        // getWhilePatternStmtNum
        std::vector<int> stmtNums = pkbFacade->getWhilePatternStmtNum("z");
        std::vector<int> expectedStmtNums = {7, 11, 4, 8};
        REQUIRE(checkVecValuesEqual(stmtNums, expectedStmtNums));

        std::vector<int> allStmtNums = pkbFacade->getWhilePatternStmtNum();
        std::vector<int> expectedAllStmtNums = {7, 11, 4, 8, 15};
        REQUIRE(checkVecValuesEqual(allStmtNums, expectedAllStmtNums));

        // getWhilePatternVariable
        std::vector<std::string> variables = pkbFacade->getWhilePatternVariable(7);
        std::vector<std::string> expectedVariables = {"z", "w"};
        REQUIRE(checkVecValuesEqual(variables, expectedVariables));

        std::vector<std::string> allVariables = pkbFacade->getWhilePatternVariable();
        std::vector<std::string> expectedAllVariables = {"z", "w", "g"};
        REQUIRE(checkVecValuesEqual(allVariables, expectedAllVariables));

        // hasWhilePattern
        bool hasWhilePatternPostInsertion = pkbFacade->hasWhilePattern();
        REQUIRE(hasWhilePatternPostInsertion == true);

        // containsWhilePattern
        bool containsWhilePattern = pkbFacade->containsWhilePattern("z", 7);
        REQUIRE(containsWhilePattern == true);

        // containsWhilePatternStmt
        bool containsWhilePatternStmt = pkbFacade->containsWhilePatternStmt(7);
        REQUIRE(containsWhilePatternStmt == true);

        // containsWhilePatternVariable
        bool containsWhilePatternVariable = pkbFacade->containsWhilePatternVariable("z");
        REQUIRE(containsWhilePatternVariable == true);
    }
}