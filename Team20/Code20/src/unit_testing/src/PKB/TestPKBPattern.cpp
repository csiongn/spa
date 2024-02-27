#include "PKB/manager/pattern/PatternManager.h"
#include "catch.hpp"
#include "PKB/Database.h"
#include "../TestUtils.h"


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
    xPlusTwo->print();
    xPlusTwoDup->print();
    REQUIRE(dupHashValue==expectedHashValue);
    // override == operator, this is pointer wont be equal
    REQUIRE(xPlusTwoDup!=xPlusTwo);
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
}