#include <memory>
#include <set>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <string>

#include "../lib/catch.hpp"
#include "MockPKBWriter.h"

#include "SP/AST.h"
#include "SP/DesignExtractor.h"

TEST_CASE("Simple program extractDesign") {

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
  auto procedure = std::make_shared<ProcedureNode>("main", blockNodeProc);
  auto program = std::make_shared<ProgramNode>(std::vector{procedure});

  // Initialize design extractor
  auto pkbWriter = std::make_shared<MockPKBWriter>();
  auto extractor = DesignExtractor(pkbWriter);
  extractor.extractDesign(*program);

  SECTION("Check correct follows relationships") {
	auto &follows = extractor.getFollows();
	REQUIRE(follows.at(2) == 1); // Follows(1, 2)
	REQUIRE(follows.at(4) == 3); // Follows(3, 4)
	REQUIRE(follows.at(6) == 2); // Follows(2, 6)
	REQUIRE_THROWS(follows.at(1));   // Follows(s, 1) should not exist
	REQUIRE_THROWS(follows.at(5));   // Follows(s, 5) should not exist
	REQUIRE_THROWS(follows.at(7));   // Follows(s, 7) should not exist
  }

  SECTION("Check correct followsT relationships") {
	auto &followsT = extractor.getFollowsT();
	auto &followsT_2 = followsT.at(2); // Get all statements that follow stmt 2 transitively
	auto &followsT_3 = followsT.at(3); // Get all statements that follow stmt 3 transitively

	REQUIRE_THROWS(followsT.at(4)); // No statement followsT statement 4
	REQUIRE_THROWS(followsT.at(5)); // No statement followsT statement 5
	REQUIRE_THROWS(followsT.at(6)); // No statement followsT statement 6

	REQUIRE(followsT_2.size() == 1); // one statement followsT statement 2
	REQUIRE(followsT_3.size() == 1); // one statement followsT statement 3
  }

  SECTION("Check corrent uses relationships") {

	// Uses relationships for statements
	auto &uses = extractor.getUses();
	std::unordered_set<std::string> stmt1_uses_expected = {"a"};
	std::unordered_set<std::string> stmt2_uses_expected = {"a", "x", "y"};
	std::unordered_set<std::string> stmt4_uses_expected = {"x"};
	std::unordered_set<std::string> stmt5_uses_expected = {"y"};
	std::unordered_set<std::string> stmt6_uses_expected = {"a", "z"};
	std::unordered_set<std::string> stmt7_uses_expected = {"a"};

	REQUIRE(stmt1_uses_expected == uses.at(1));
	REQUIRE(stmt2_uses_expected == uses.at(2));
	REQUIRE(stmt4_uses_expected == uses.at(4));
	REQUIRE(stmt5_uses_expected == uses.at(5));
	REQUIRE(stmt6_uses_expected == uses.at(6));
	REQUIRE(stmt7_uses_expected == uses.at(7));

	REQUIRE_THROWS(uses.at(3));

	// Uses relationships for procedures

	auto &procUses = extractor.getProcUses(); // Retrieve relationships from SP Extractor
	auto &pkbProcUses = pkbWriter->procUsesRelations;
	std::unordered_set<std::string> main_uses_expected = {"a", "x", "y", "z"};

	REQUIRE(main_uses_expected == procUses.at("main"));
	REQUIRE(main_uses_expected == pkbProcUses.at("main"));

	REQUIRE_THROWS(procUses.at("procedure"));
	REQUIRE_THROWS(pkbProcUses.at("procedure"));
  }

  SECTION("Check corrent parent relationships") {
	auto &parent = extractor.getParent();

	REQUIRE(parent.at(3) == 2); // Parent(2, 3)
	REQUIRE(parent.at(4) == 2); // Parent(2, 4)
	REQUIRE(parent.at(5) == 2); // Parent(2, 5)
	REQUIRE(parent.at(7) == 6); // Parent(6, 7)
	REQUIRE_THROWS(parent.at(1));   // Parent(s, 1) should not exist
	REQUIRE_THROWS(parent.at(2));   // Parent(s, 2) should not exist
	REQUIRE_THROWS(parent.at(6));   // Parent(s, 6) should not exist
  }

  SECTION("Check corrent parentT relationships") {
	auto &parentT = extractor.getParentT();
	auto &parentT_2 = parentT.at(2);
	auto &parentT_6 = parentT.at(6);

	REQUIRE(parentT_2.size() == 3);
	REQUIRE(parentT_6.size() == 1);
  }

  SECTION("Check corrent modifies relationships") {

	// Modifies relationship for statements
	auto &modifies = extractor.getModifies();
	std::unordered_set<std::string> stmt1_modifies_expected = {"a"};
	std::unordered_set<std::string> stmt2_modifies_expected = {"x"};
	std::unordered_set<std::string> stmt3_modifies_expected = {"x"};
	std::unordered_set<std::string> stmt6_modifies_expected = {"b"};
	std::unordered_set<std::string> stmt7_modifies_expected = {"b"};

	REQUIRE(stmt1_modifies_expected == modifies.at(1));
	REQUIRE(stmt2_modifies_expected == modifies.at(2));
	REQUIRE(stmt3_modifies_expected == modifies.at(3));
	REQUIRE(stmt6_modifies_expected == modifies.at(6));
	REQUIRE(stmt7_modifies_expected == modifies.at(7));

	REQUIRE_THROWS(modifies.at(4));
	REQUIRE_THROWS(modifies.at(5));

	// Modifies relationships for procedures
	auto &procModifies = extractor.getProcModifies();
	auto &pkbProcModifies = pkbWriter->procModifiesRelations;
	std::unordered_set<std::string> main_modifies_expected = {"a", "x", "b"};

	REQUIRE(main_modifies_expected == procModifies.at("main"));
	REQUIRE(main_modifies_expected == pkbProcModifies.at("main"));

	REQUIRE_THROWS(procModifies.at("procedure"));
	REQUIRE_THROWS(pkbProcModifies.at("procedure"));
  }

  SECTION("Check corrent if condition variables") {
	auto &ifStmts = extractor.getIfStmts();

	std::unordered_set<std::string> stmt2_if_control_variables_expected = {"a"};

	REQUIRE(ifStmts.size() == 1);
	REQUIRE(stmt2_if_control_variables_expected == ifStmts.at(2));
  }

  SECTION("Check corrent while condition variables") {
	auto &whileStmts = extractor.getWhileStmts();

	std::unordered_set<std::string> stmt6_while_control_variables_expected = {"a", "z"};

	REQUIRE(whileStmts.size() == 1);
	REQUIRE(stmt6_while_control_variables_expected == whileStmts.at(6));
  }

  //
  //   procedure foo {
  // 8.   i = 3;
  //   }
  std::shared_ptr<StmtNode> assignStmt3 = std::make_shared<AssignNode>(8, "i", three);
  auto stmtLst4 = {assignStmt3};
  auto blockNode4 = std::make_shared<BlockNode>(stmtLst4);
  auto procedureFoo = std::make_shared<ProcedureNode>("foo", blockNode4);

  auto newProgam = std::make_shared<ProgramNode>(std::vector{procedure, procedureFoo});

  // Initialize new design extractor
  auto newPKBWriter = std::make_shared<MockPKBWriter>();
  auto newExtractor = DesignExtractor(newPKBWriter);
  newExtractor.extractDesign(*newProgam);

  SECTION("Check corrent uses relationships") {

	// Uses relationships for statements
	auto &newUses = newExtractor.getUses();
	std::unordered_set<std::string> stmt1_uses_expected = {"a"};
	std::unordered_set<std::string> stmt2_uses_expected = {"a", "x", "y"};
	std::unordered_set<std::string> stmt4_uses_expected = {"x"};
	std::unordered_set<std::string> stmt5_uses_expected = {"y"};
	std::unordered_set<std::string> stmt6_uses_expected = {"a", "z"};
	std::unordered_set<std::string> stmt7_uses_expected = {"a"};

	REQUIRE(stmt1_uses_expected == newUses.at(1));
	REQUIRE(stmt2_uses_expected == newUses.at(2));
	REQUIRE(stmt4_uses_expected == newUses.at(4));
	REQUIRE(stmt5_uses_expected == newUses.at(5));
	REQUIRE(stmt6_uses_expected == newUses.at(6));
	REQUIRE(stmt7_uses_expected == newUses.at(7));

	REQUIRE_THROWS(newUses.at(3));

	// Uses relationships for procedures

	auto &newProcUses = newExtractor.getProcUses(); // Retrieve relationships from SP Extractor
	auto &newPKBProcUses = newPKBWriter->procUsesRelations;
	std::unordered_set<std::string> main_uses_expected = {"a", "x", "y", "z"};
	std::unordered_set<std::string> foo_uses_expected = {};

	REQUIRE(main_uses_expected == newProcUses.at("main"));
	REQUIRE(main_uses_expected == newPKBProcUses.at("main"));
	REQUIRE(foo_uses_expected == newProcUses.at("foo"));
	// Mock PKB does not contain uses relationships inside 'foo' procedure because it only tracks
	// such procedures when the insertUsesProc method is called.

	REQUIRE_THROWS(newProcUses.at("procedure"));
	REQUIRE_THROWS(newPKBProcUses.at("procedure"));
  }

  SECTION("Check corrent modifies relationships") {

	// Modifies relationship for statements
	auto &newModifies = extractor.getModifies();
	std::unordered_set<std::string> stmt1_modifies_expected = {"a"};
	std::unordered_set<std::string> stmt2_modifies_expected = {"x"};
	std::unordered_set<std::string> stmt3_modifies_expected = {"x"};
	std::unordered_set<std::string> stmt6_modifies_expected = {"b"};
	std::unordered_set<std::string> stmt7_modifies_expected = {"b"};

	REQUIRE(stmt1_modifies_expected == newModifies.at(1));
	REQUIRE(stmt2_modifies_expected == newModifies.at(2));
	REQUIRE(stmt3_modifies_expected == newModifies.at(3));
	REQUIRE(stmt6_modifies_expected == newModifies.at(6));
	REQUIRE(stmt7_modifies_expected == newModifies.at(7));

	REQUIRE_THROWS(newModifies.at(4));
	REQUIRE_THROWS(newModifies.at(5));

	// Modifies relationships for procedures
	auto &newProcModifies = newExtractor.getProcModifies();
	auto &newPKBProcModifies = newPKBWriter->procModifiesRelations;
	std::unordered_set<std::string> main_modifies_expected = {"a", "x", "b"};
	std::unordered_set<std::string> foo_modifies_expected = {"i"};

	REQUIRE(main_modifies_expected == newProcModifies.at("main"));
	REQUIRE(main_modifies_expected == newPKBProcModifies.at("main"));
	REQUIRE(foo_modifies_expected == newProcModifies.at("foo"));
	REQUIRE(foo_modifies_expected == newPKBProcModifies.at("foo"));

	REQUIRE_THROWS(newProcModifies.at("procedure"));
	REQUIRE_THROWS(newPKBProcModifies.at("procedure"));
  }
}