#include "SP/AST.h"
#include "SP/DesignExtractor.h"
#include "SP/control_flow_graph/CFGBuilder.h"

#include "../lib/catch.hpp"
#include ".././MockPKBWriter.h"

TEST_CASE("Simple program") {
	// Create assignment statement: a = a + 3 * 4;
	auto three = std::make_shared<LiteralNode>("3");
	auto four = std::make_shared<LiteralNode>("4");
	auto threeTimesFour = std::make_shared<BinaryExprNode>(three, "*", four);
	auto a = std::make_shared<VariableNode>("a");
	auto aPlusThreeTimesFour = std::make_shared<BinaryExprNode>(a, "+", threeTimesFour);
	auto assignStmt = std::make_shared<AssignNode>(1, "a", aPlusThreeTimesFour);

	// Create statement block
	// read x;
	std::shared_ptr<StmtNode> readStmt = std::make_shared<ReadNode>(3, "x");

	// print x;
	std::shared_ptr<StmtNode> printStmt = std::make_shared<PrintNode>(4, "x");

	auto stmtLst = {readStmt, printStmt};
	auto blockNode = std::make_shared<BlockNode>(stmtLst);

	// Create another statement block
	std::shared_ptr<StmtNode> printStmt2 = std::make_shared<PrintNode>(5, "y");
	auto stmtLst2 = {printStmt2};
	auto blockNode2 = std::make_shared<BlockNode>(stmtLst2);

	// Create if node containing previous two blocks
	auto aGreater3 = std::make_shared<LogicalOpNode>(a, ">", three);
	auto ifStmt = std::make_shared<IfNode>(2, aGreater3,
										   blockNode,
										   blockNode2);

	// Assign b = a + 3 * 4
	std::shared_ptr<StmtNode> assignStmt2 = std::make_shared<AssignNode>(7, "b", aPlusThreeTimesFour);
	auto stmtLst3 = {assignStmt2};
	auto blockNode3 = std::make_shared<BlockNode>(stmtLst3);

	// Create condition z < z + 3 || !(a > 3)
	auto z = std::make_shared<VariableNode>("z");
	auto zDup = std::make_shared<VariableNode>("z");
	auto threeDup = std::make_shared<LiteralNode>("3");
	auto zPlusThree = std::make_shared<BinaryExprNode>(z, "+", threeDup);
	auto zLessZPlusThree = std::make_shared<RelExprNode>(z, "<", zPlusThree);
	auto notAGreaterThree = std::make_shared<NegationNode>(aGreater3);

	auto zLessZPlusThreeOrNotAGreaterThree = std::make_shared<LogicalOpNode>(zLessZPlusThree, "||", notAGreaterThree);

	auto whileStmt = std::make_shared<WhileNode>(6, zLessZPlusThreeOrNotAGreaterThree, blockNode3);

	// Create procedure "main" with previous three statements
	std::vector<std::shared_ptr<StmtNode>> procStmtLst = {assignStmt, ifStmt, whileStmt};
	std::shared_ptr<BlockNode> blockNodeProc = std::make_shared<BlockNode>(procStmtLst);
	auto procedureMain = std::make_shared<ProcedureNode>("main", blockNodeProc);

	std::shared_ptr<StmtNode> assignStmt3 = std::make_shared<AssignNode>(8, "i", three);
	auto stmtLst4 = {assignStmt3};
	auto blockNode4 = std::make_shared<BlockNode>(stmtLst4);
	auto procedureFoo = std::make_shared<ProcedureNode>("foo", blockNode4);

	auto program = std::make_shared<ProgramNode>(std::vector{procedureMain, procedureFoo});

	// Initialize CFGBuilder
	auto cfgManager = CFGBuilder::buildCFG(*program);

	SECTION("Check correct dimensions") {
		REQUIRE(cfgManager->getCFGs().size() == 2);
	}
}