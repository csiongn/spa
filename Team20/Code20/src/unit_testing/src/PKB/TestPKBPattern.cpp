#include "PKB/manager/pattern/PatternManager.h"
#include "catch.hpp"
#include "PKB/Database.h"
#include "../TestUtils.h"
#include "PKB/PKB.h"
#include "SP/AST.h"

TEST_CASE("PKB Pattern Test") {
  // initialise database
  auto db = std::make_shared<Database>();
  // Create Nodes
  uint16_t stmtNum = 5;
  uint16_t dupStmtNum = 8;
  std::shared_ptr<ExprNode> x = std::make_shared<VariableNode>("x");
  std::shared_ptr<ExprNode> two = std::make_shared<LiteralNode>("2");
  std::shared_ptr<ExprNode> four = std::make_shared<LiteralNode>("4");
  std::shared_ptr<ExprNode> xPlusTwo = std::make_shared<BinaryExprNode>(x, "+", two);
  std::shared_ptr<ExprNode> xPlusFour = std::make_shared<BinaryExprNode>(x, "+", four);
  std::shared_ptr<StmtNode> assignStmt = std::make_shared<AssignNode>(stmtNum, "x", xPlusTwo);
  // create duplicate of x and two
  std::shared_ptr<ExprNode> xDup = std::make_shared<VariableNode>("x");
  std::shared_ptr<ExprNode> twoDup = std::make_shared<LiteralNode>("2");
  std::shared_ptr<ExprNode> xPlusTwoDup = std::make_shared<BinaryExprNode>(xDup, "+", twoDup);
  auto dupHashValue = xPlusTwoDup->getHashValue();
  auto expectedHashValue = xPlusTwo->getHashValue();
  REQUIRE(dupHashValue == expectedHashValue);
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
	// Sub expressions does not exist if checking for exact pattern
	REQUIRE(!assignPatternManager->containsPattern("x", two->getHashValue()));

	// test getAllRHSNodePtrs
	auto result3 = assignPatternManager->getRHSNodePtr();
	REQUIRE(checkVecValuesEqual(result3, std::vector<std::shared_ptr<ExprNode>>{xPlusTwo, xPlusTwoDup}));

	// same hash get 2 node Pointers
	auto result4 = assignPatternManager->getRHSNodePtr(xPlusTwo->getHashValue());
	REQUIRE(checkVecValuesEqual(result4, std::vector<std::shared_ptr<ExprNode>>{xPlusTwo, xPlusTwoDup}));
  }

  SECTION("AssignPartialPatternManager") {
	// T, U need to be var : string and U is ExprNode's Hash size_t, V is ptr
	// LHS Variable Name, RHS Node Hash Value, Ptr to Node
	// LHS variable
	std::string varA = "a";
	// Test direct assignPatternManager first
	auto assignPartialPatternManager = db->getAssignPartialPatternManager();
	// Insert
	assignPartialPatternManager->insert(varA, xPlusTwo->getHashValue(), stmtNum, xPlusTwo);
	// Insert duplicate
	assignPartialPatternManager->insert(varA, dupHashValue, dupStmtNum, xPlusTwoDup);
	// Insert subtrees of full expression
	// Insert x, 2 leaf node in
	assignPartialPatternManager->insert(varA, x->getHashValue(), stmtNum, x);
	assignPartialPatternManager->insert(varA, two->getHashValue(), stmtNum, two);

	// Get
	auto result = assignPartialPatternManager->getRHS(varA);
	REQUIRE(checkVecValuesEqual(result,
								std::vector<size_t>{xPlusTwo->getHashValue(), x->getHashValue(), two->getHashValue()}));
	auto result2 = assignPartialPatternManager->getLHS(xPlusTwo->getHashValue());
	REQUIRE(result2 == std::vector<std::string>{varA});

	// Test contains
	REQUIRE(assignPartialPatternManager->containsPattern(varA, xPlusTwo->getHashValue()));
	REQUIRE(assignPartialPatternManager->containsPattern(varA, dupHashValue));

	// test getAllRHSNodePtrs
	auto result3 = assignPartialPatternManager->getRHSNodePtr();
	REQUIRE(checkVecValuesEqual(result3, std::vector<std::shared_ptr<ExprNode>>{xPlusTwo, xPlusTwoDup, x, two}));

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
	std::shared_ptr<ExprNode> yPlusZTimesT = std::make_shared<BinaryExprNode>(y, "+", zTimesT);
	std::shared_ptr<ExprNode> xTimesYPlusZTimesT = std::make_shared<BinaryExprNode>(xTimesY, "+", zTimesT);

	// LHS variables
	std::string varX = "x";
	std::string varY = "y";
	// https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/program-query-language/assign-pattern-clause.html
	// Insert subtrees as well as full expression into AssignPartialPatternManager
	// varX = v + x * y + z * t
	// LHS: "x"
	// "v"
	assignPartialPatternManager->insert(varX, v->getHashValue(), stmtNum, v);
	// "x"
	assignPartialPatternManager->insert(varX, x->getHashValue(), stmtNum, x);
	// "y"
	assignPartialPatternManager->insert(varX, y->getHashValue(), stmtNum, y);
	// "z"
	assignPartialPatternManager->insert(varX, z->getHashValue(), stmtNum, z);
	// "t"
	assignPartialPatternManager->insert(varX, t->getHashValue(), stmtNum, t);
	// "x * y"
	assignPartialPatternManager->insert(varX, xTimesY->getHashValue(), stmtNum, xTimesY);
	// "z * t"
	assignPartialPatternManager->insert(varX, zTimesT->getHashValue(), stmtNum, zTimesT);
	// "v + x * y"
	assignPartialPatternManager->insert(varX, vPlusXTimesY->getHashValue(), stmtNum, vPlusXTimesY);
	// "v + x * y + z * t"
	assignPartialPatternManager->insert(varX,
										vPlusXTimesYPlusZTimesT->getHashValue(),
										stmtNum,
										vPlusXTimesYPlusZTimesT);
	// "x * y + z * t" is not a sub-expression on RHS according to website

	// Test do not contains for different RHS subtrees/sub-expressions
	REQUIRE(assignPartialPatternManager->containsRHS(vPlusX->getHashValue()) == false);
	REQUIRE(!vPlusXTimesYPlusZTimesT->containsSubtree(*vPlusX));
	REQUIRE(assignPartialPatternManager->containsRHS(yPlusZTimesT->getHashValue()) == false);
	REQUIRE(!vPlusXTimesYPlusZTimesT->containsSubtree(*yPlusZTimesT));
	REQUIRE(assignPartialPatternManager->containsRHS(xTimesYPlusZTimesT->getHashValue()) == false);
	REQUIRE(!vPlusXTimesYPlusZTimesT->containsSubtree(*xTimesYPlusZTimesT));
	// Test contains for RHS subtrees tgt with ExprNode containsSubtree
	REQUIRE(assignPartialPatternManager->containsRHS(v->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*v));
	REQUIRE(assignPartialPatternManager->containsRHS(x->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*x));
	REQUIRE(assignPartialPatternManager->containsRHS(y->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*y));
	REQUIRE(assignPartialPatternManager->containsRHS(z->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*z));
	REQUIRE(assignPartialPatternManager->containsRHS(t->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*t));

	REQUIRE(assignPartialPatternManager->containsRHS(xTimesY->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*xTimesY));
	REQUIRE(assignPartialPatternManager->containsRHS(zTimesT->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*zTimesT));
	REQUIRE(assignPartialPatternManager->containsRHS(vPlusXTimesY->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*vPlusXTimesY));
	REQUIRE(assignPartialPatternManager->containsRHS(vPlusXTimesYPlusZTimesT->getHashValue()) == true);
	REQUIRE(vPlusXTimesYPlusZTimesT->containsSubtree(*vPlusXTimesYPlusZTimesT));

	// Test contains for different LHS variables
	REQUIRE(assignPartialPatternManager->containsLHS(varX) == true);
	REQUIRE(assignPartialPatternManager->containsLHS(varY) == false);
  }

  SECTION("PKBFacade - AssignPartialPatternManager") {
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	auto twoHash = two->getHashValue();
	auto xHash = x->getHashValue();
	auto xPlusTwoHash = xPlusTwo->getHashValue();

	bool hasAssignPartialPattern = pkbFacade->hasAssignPartialPattern();
	REQUIRE(hasAssignPartialPattern == false);

	// insertAssignPartialPattern
	// Insert 2 lines of stmts
	// x = x + 2
	pkbFacade->insertAssignPartialPattern("x", xPlusTwoHash, 3, xPlusTwo);
	pkbFacade->insertAssignPartialPattern("x", xHash, 3, x);
	pkbFacade->insertAssignPartialPattern("x", twoHash, 3, two);
	// x = 2
	pkbFacade->insertAssignPartialPattern("x", twoHash, 10, two);
	// y = 2
	pkbFacade->insertAssignPartialPattern("y", twoHash, 15, two);

	// getAssignPartialPatternLHS
	// two exist in 3 stmts
	std::vector<std::string> lhs = pkbFacade->getAssignPartialPatternLHS(twoHash);
	std::vector<std::string> expectedLHS = {"x", "y"};
	REQUIRE(checkVecValuesEqual(lhs, expectedLHS));

	// getAssignPartialPatternRHS
	// x exist in 2 stmts
	std::vector<size_t> rhs = pkbFacade->getAssignPartialPatternRHS("x");
	std::vector<size_t> expectedRHS = {xPlusTwoHash, twoHash, xHash};
	REQUIRE(checkVecValuesEqual(rhs, expectedRHS));

	// getAssignPartialPatternLHS
	// get all LHS variables
	std::vector<std::string> allLHS = pkbFacade->getAssignPartialPatternLHS();
	std::vector<std::string> expectedAllLHS = {"x", "y"};
	REQUIRE(checkVecValuesEqual(allLHS, expectedAllLHS));

	// getAssignPartialPatternRHS
	// get all RHS hash
	std::vector<size_t> allRHS = pkbFacade->getAssignPartialPatternRHS();
	std::vector<size_t> expectedAllRHS = {xPlusTwoHash, xHash, twoHash};

	// getAssignPartialPatternStmtNum
	std::vector<int> stmtNums = pkbFacade->getAssignPartialPatternStmtNum("x", twoHash);
	std::vector<int> expectedStmtNums = {3, 10};
	REQUIRE(checkVecValuesEqual(stmtNums, expectedStmtNums));

	// getAssignPartialPatternLHSStmtNum
	// get stmt num for LHS variable
	std::vector<int> stmtNumsLHS = pkbFacade->getAssignPartialPatternLHSStmtNum("x");
	std::vector<int> expectedStmtNumsLHS = {3, 10};
	REQUIRE(checkVecValuesEqual(stmtNumsLHS, expectedStmtNumsLHS));

	// getAssignPartialPatternRHSStmtNum
	// get stmt num for RHS hash
	std::vector<int> stmtNumsRHS = pkbFacade->getAssignPartialPatternRHSStmtNum(twoHash);
	std::vector<int> expectedStmtNumsRHS = {3, 10, 15};
	REQUIRE(checkVecValuesEqual(stmtNumsRHS, expectedStmtNumsRHS));

	// getAssignPartialPatternRHSExprNodePtr
	// get ExprNode ptr for RHS hash
	std::vector<std::shared_ptr<ExprNode>> exprNodePtrs = pkbFacade->getAssignPartialPatternRHSExprNodePtr(twoHash);
	std::vector<std::shared_ptr<ExprNode>> expectedExprNodePtrs = {two};
	REQUIRE(checkVecValuesEqual(exprNodePtrs, expectedExprNodePtrs));

	// getAssignPartialPatternRHSExprNodePtr
	// get all ExprNode ptr
	std::vector<std::shared_ptr<ExprNode>> allExprNodePtrs = pkbFacade->getAssignPartialPatternRHSExprNodePtr();
	std::vector<std::shared_ptr<ExprNode>> expectedAllExprNodePtrs = {xPlusTwo, x, two};
	REQUIRE(checkVecValuesEqual(allExprNodePtrs, expectedAllExprNodePtrs));

	// containsAssignPartialPattern
	bool containsAssignPartialPattern = pkbFacade->containsAssignPartialPattern("x", xPlusTwoHash);
	REQUIRE(containsAssignPartialPattern == true);
	bool containsAssignPartialPatternFalse = pkbFacade->containsAssignPartialPattern("y", xPlusTwoHash);
	REQUIRE(containsAssignPartialPatternFalse == false);

	// containsAssignPartialPatternLHS
	bool containsAssignPartialPatternLHS = pkbFacade->containsAssignPartialPatternLHS("x");
	REQUIRE(containsAssignPartialPatternLHS == true);

	// containsAssignPartialPatternRHS
	// TODO: use this to check for collisions, need to create alot of hashes
	bool containsAssignPartialPatternRHS = pkbFacade->containsAssignPartialPatternRHS(xPlusTwoHash);
	REQUIRE(containsAssignPartialPatternRHS == true);
	bool containsAssignPartialPatternRHSFalse = pkbFacade->containsAssignPartialPatternRHS(xPlusFour->getHashValue());
	REQUIRE(containsAssignPartialPatternRHSFalse == false);

	// hasAssignPartialPattern
	bool hasAssignPartialPatternPostInsertion = pkbFacade->hasAssignPartialPattern();
	REQUIRE(hasAssignPartialPatternPostInsertion == true);
  }

  SECTION("PKBFacade - IfPatternManager") {
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

  SECTION("PKBFacade - WhilePatternManager") {
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
	std::unordered_set<int> stmtSet = {4, 8};
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