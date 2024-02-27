#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "SP/AST.h"

#include "catch.hpp"


TEST_CASE("ASTNode") {
    // test if Nodes can do sub tree and tree matching
    // Create Nodes
    uint16_t stmtNum = 5;
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

    SECTION("SubTree Matching") {
        // Test SubTree Matching
        // x + 2
        REQUIRE(xPlusTwo->containsSubtree(*xPlusTwoDup));
        // x
        REQUIRE(xPlusTwoDup->containsSubtree(*x));
        // 2
        // x + 2
        REQUIRE(xPlusTwo->containsSubtree(*two));
    }


    // create larger RHS expression
    // v + x * y + z * t
    std::shared_ptr<ExprNode> v = std::make_shared<VariableNode>("v");
    std::shared_ptr<ExprNode> y = std::make_shared<VariableNode>("y");
    std::shared_ptr<ExprNode> z = std::make_shared<VariableNode>("z");
    std::shared_ptr<ExprNode> t = std::make_shared<VariableNode>("t");
    std::shared_ptr<ExprNode> xTimesY = std::make_shared<BinaryExprNode>(x, "*", y);
    std::shared_ptr<ExprNode> zTimesT = std::make_shared<BinaryExprNode>(z, "*", t);
    std::shared_ptr<ExprNode> vPlusXTimesY = std::make_shared<BinaryExprNode>(v, "+", xTimesY);
    std::shared_ptr<ExprNode> vPlusXTimesYPlusZTimesT = std::make_shared<BinaryExprNode>(vPlusXTimesY, "+", zTimesT);
    // Duplicate of v + x * y + z * t
    std::shared_ptr<ExprNode> vPlusXTimesYPlusZTimesTDup = std::make_shared<BinaryExprNode>(vPlusXTimesY, "+", zTimesT);

    // Does not contain subtree
    std::shared_ptr<ExprNode> vPlusX = std::make_shared<BinaryExprNode>(v, "+", x);
    std::shared_ptr<ExprNode> yPlusZTimesT= std::make_shared<BinaryExprNode>(y, "+", zTimesT);
    std::shared_ptr<ExprNode> xTimesYPlusZTimesT = std::make_shared<BinaryExprNode>(xTimesY, "+", zTimesT);

    SECTION("Sub tree matching for v+x*y+z*t") {
        // EXACT
        // REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*vPlusXTimesYPlusZTimesT));
        // PARTIAL
        // Print out each serialise and hash value to check
        REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*v));
        REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*xTimesY));
        REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*zTimesT));
        REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*vPlusXTimesY));
        REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*vPlusXTimesYPlusZTimesTDup));
        // False
        REQUIRE(!vPlusXTimesYPlusZTimesT->containsSubtree(*vPlusX));
        REQUIRE(!vPlusXTimesYPlusZTimesT->containsSubtree(*yPlusZTimesT));
        REQUIRE(!vPlusXTimesYPlusZTimesT->containsSubtree(*xTimesYPlusZTimesT));
    }
}