#include "PKB/manager/relationship/RelationshipManager.h"
#include "PKB/manager/relationship/FollowsManager.h"
#include "catch.hpp"
#include "PKB/Database.h"
#include "PKB/PKB.h"
#include "../TestUtils.h"

TEST_CASE("PKB Relationship Test") {
  // initialise database
  auto db = Database();

  SECTION("RelationshipManager") {
	// Create an instance of RelationshipManager
	auto sampleRelationshipManager = RelationshipManager<int, std::string>();

	// hasRelationship
	bool hasRelationship = sampleRelationshipManager.hasRelationship();
	REQUIRE(hasRelationship == false);
	// Insert some key-value pairs
	sampleRelationshipManager.insert(1, "apple");
	sampleRelationshipManager.insert(2, "banana");
	sampleRelationshipManager.insert(1, "orange");
	sampleRelationshipManager.insert(3, "apple");

	// Get values associated with a key
	std::vector<std::string> values = sampleRelationshipManager.get(1);
	std::vector<std::string> expectedValues = {"apple", "orange"};
	REQUIRE(checkVecValuesEqual(values, expectedValues));

	// Get keys associated with a value
	std::vector<int> keys = sampleRelationshipManager.getReverse("apple");
	std::vector<int> expectedKeys = {1, 3};
	REQUIRE(checkVecValuesEqual(keys, expectedKeys));

	// Check if a key exists
	bool containsKey = sampleRelationshipManager.contains(2);
	REQUIRE(containsKey == true);

	// Check if a value exists
	bool containsValue = sampleRelationshipManager.containsReverse("banana");
	REQUIRE(containsValue == true);

	// Check if a value already exists in the unordered_set of data
	bool containsRelationship = sampleRelationshipManager.containsRelationship(1, "apple");
	REQUIRE(containsRelationship == true);
	// Will not be able to insert the same value again and return normally, list should be the same
	sampleRelationshipManager.insert(1, "apple");
	std::vector<std::string> valuesAfterInsert = sampleRelationshipManager.get(1);
	REQUIRE(checkVecValuesEqual(valuesAfterInsert, expectedValues));

	// getKeys()
	auto keysVec = sampleRelationshipManager.getKeys();
	std::vector<int> expectedKeysVec = {1, 2, 3};
	REQUIRE(checkVecValuesEqual(keysVec, expectedKeysVec));

	// getValues()
	std::vector<std::string> valuesVec = sampleRelationshipManager.getValues();
	std::vector<std::string> expectedValuesVec = {"apple", "banana", "orange"};
  }

  SECTION("FollowsManager") {
	// Create an instance of FollowsManager
	auto followsManager = FollowsManager();

	// hasRelationship
	bool hasRelationship = followsManager.hasRelationship();
	REQUIRE(hasRelationship == false);

	// Insert some key-value pairs
	followsManager.insert(1, 2);
	followsManager.insert(2, 3);
	followsManager.insert(3, 4);

	// Get values associated with a key
	std::vector<int> values = followsManager.get(1);
	std::vector<int> expectedValues = {2};
	REQUIRE(checkVecValuesEqual(values, expectedValues));
	// Get keys associated with a value
	std::vector<int> keys = followsManager.getReverse(3);
	std::vector<int> expectedKeys = {2};
	REQUIRE(checkVecValuesEqual(keys, expectedKeys));

	// Check if a key exists
	bool containsKey = followsManager.contains(2);
	REQUIRE(containsKey == true);

	// Check if a value exists
	bool containsValue = followsManager.containsReverse(4);
	REQUIRE(containsValue == true);
  }

  SECTION("PKBFacade - FollowsManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// Insert some key-value pairs
	// insertFollows
	pkbFacade->insertFollows(1, 2);
	pkbFacade->insertFollows(2, 3);

	// Get values associated with a key
	// Query: Select s such that Follows(1, s)
	// getFollows
	std::vector<int> values = pkbFacade->getFollows(1);
	std::vector<int> expectedValues = {2};
	REQUIRE(checkVecValuesEqual(values, expectedValues));

	// containsFollows
	bool containsFollows = pkbFacade->containsFollows(10);
	REQUIRE(containsFollows == false);

	// containsFollowing
	bool containsFollowing = pkbFacade->containsFollowing(2);
	REQUIRE(containsFollowing == true);

	// containsFollowsRelationship
	bool containsFollowsRelationship = pkbFacade->containsFollowsRelationship(1, 2);
	REQUIRE(containsFollowsRelationship == true);
  }

  SECTION("PKBFacade - FollowsTManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// hasFollowsTRelationship
	bool hasFollowsTRelationship = pkbFacade->hasFollowsTRelationship();
	REQUIRE(hasFollowsTRelationship == false);

	// Will be similar to SP inserting FollowsT relationship
	for (int i = 2; i < 10; i++) {
	  pkbFacade->insertFollowsT(1, i);
	}

	pkbFacade->insertFollowsT(2, 3);

	// Query: select s such that Follows*(1, s)
	// getFollowsT
	std::vector<int> values = pkbFacade->getFollowsT(1);
	std::vector<int> expectedValues = {2, 3, 4, 5, 6, 7, 8, 9};
	REQUIRE(checkVecValuesEqual(values, expectedValues));

	// Query: select s such that Follows*(s, 3)
	// getFollowingT, 3 follows after 1, 2 transitively
	std::vector<int> valuesFollowingT = pkbFacade->getFollowingT(3);
	std::vector<int> expectedValuesFollowingT = {1, 2};

	REQUIRE(checkVecValuesEqual(valuesFollowingT, expectedValuesFollowingT));

	// containsFollowsT
	bool containsFollowsT = pkbFacade->containsFollowsT(1);
	REQUIRE(containsFollowsT == true);

	// containsFollowingT
	bool containsFollowingT = pkbFacade->containsFollowingT(10);
	REQUIRE(containsFollowingT == false);

	// containsFollowsTRelationship
	bool containsFollowsTRelationship = pkbFacade->containsFollowsTRelationship(1, 2);
	REQUIRE(containsFollowsTRelationship == true);
  }

  SECTION("PKBFacade - ParentManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;
	// hasParentRelationship
	bool hasParentRelationship = pkbFacade->hasParentRelationship();
	REQUIRE(hasParentRelationship == false);
	// insertParent
	pkbFacade->insertParent(1, 2);

	pkbFacade->insertParent(2, 3);

	// select s such that Parent(1, s)
	// getChild
	std::vector<int> childVec = pkbFacade->getChild(1);
	std::vector<int> expectedValues = {2};
	REQUIRE(checkVecValuesEqual(childVec, expectedValues));
	// select s such that Parent(s, 3)
	// getParent
	std::vector<int> parentVec = pkbFacade->getParent(3);
	std::vector<int> expectedValuesFollowingT = {2};

	REQUIRE(checkVecValuesEqual(parentVec, expectedValuesFollowingT));
	// containsParent
	bool containsParent = pkbFacade->containsParent(2);
	REQUIRE(containsParent == true);

	// containsChild
	bool containsChild = pkbFacade->containsChild(3);
	REQUIRE(containsChild == true);

	// containsParentRelationship
	bool containsParentRelationship = pkbFacade->containsParentRelationship(1, 2);
	REQUIRE(containsParentRelationship == true);
  }

  SECTION("PKBFacade - VariableManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// insertVariable
	pkbFacade->insertVariable("x");
	pkbFacade->insertVariable("y");

	// Query: select v
	// getAllVariables
	std::vector<std::string> values = pkbFacade->getAllVariables();
	std::vector<std::string> expectedValues = {"x", "y"};
	REQUIRE(checkVecValuesEqual(values, expectedValues));

	// containsVariable
	bool containsVariableX = pkbFacade->containsVariable("x");
	REQUIRE(containsVariableX == true);

	bool containsVariableZ = pkbFacade->containsVariable("z");
	REQUIRE(containsVariableZ == false);
  }

  SECTION("PKBFacade - AssignManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// insertAssign
	pkbFacade->insertAssign(1);
	pkbFacade->insertAssign(2);

	// insertAssign with set
	std::unordered_set<int> assignSet = {3, 4, 5};
	pkbFacade->insertAssign(assignSet);
	// Query: Select a
	// getAllAssignStmtNum
	std::vector<int> values = pkbFacade->getAllAssignStmtNum();
	std::vector<int> expectedValues = {1, 2, 3, 4, 5};
	REQUIRE(checkVecValuesEqual(values, expectedValues));
  }

  SECTION("PKBFacade - UsesStmtManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// hasUsesRelationship
	bool hasUsesRelationship = pkbFacade->hasUsesRelationship();
	REQUIRE(hasUsesRelationship == false);
	// insertUses
	pkbFacade->insertUsesStmt(1, "x");
	pkbFacade->insertUsesStmt(2, "y");
	pkbFacade->insertUsesStmt(3, "z");
	// insert Set
	std::unordered_set<std::string> variableSet = {"a", "b", "c"};
	pkbFacade->insertUsesStmt(1, variableSet);

	// Query: Select v such that Uses(1, v)
	// getUses
	std::vector<std::string> variables = pkbFacade->getUsesVariable(1);
	std::vector<std::string> expectedValues = {"x", "a", "b", "c"};
	REQUIRE(checkVecValuesEqual(variables, expectedValues));

	// containsUsesStmt
	bool containsUsesStmt = pkbFacade->containsUsesStmt(1);
	REQUIRE(containsUsesStmt == true);

	// containsUsesVariable
	bool containsUsesVariable = pkbFacade->containsUsesVariable("z");
	REQUIRE(containsUsesVariable == true);

	// containsUsesRelationship
	bool containsUsesRelationship = pkbFacade->containsUsesRelationship(1, "a");
	REQUIRE(containsUsesRelationship == true);
  }

  SECTION("PKBFacade - ModifiesStmtManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// hasModifiesRelationship
	bool hasModifiesRelationship = pkbFacade->hasModifiesRelationship();
	REQUIRE(hasModifiesRelationship == false);

	// insertModifies
	pkbFacade->insertModifiesStmt(1, "x");
	pkbFacade->insertModifiesStmt(2, "y");
	pkbFacade->insertModifiesStmt(3, "z");
	// insert Set
	std::unordered_set<std::string> variableSet = {"a", "b", "c"};
	pkbFacade->insertModifiesStmt(4, variableSet);

	// Query: Select v such that Modifies(1, v)
	// getModifies
	std::vector<std::string> variables = pkbFacade->getModifiesVariable(4);
	std::vector<std::string> expectedValues = {"a", "b", "c"};
	REQUIRE(checkVecValuesEqual(variables, expectedValues));

	// containsModifiesStmt
	bool containsModifiesStmt = pkbFacade->containsModifiesStmt(1);
	REQUIRE(containsModifiesStmt == true);

	// containsModifiesVariable
	bool containsModifiesVariable = pkbFacade->containsModifiesVariable("a");
	REQUIRE(containsModifiesVariable == true);

	// getModifiesStmt
	std::vector<int> mStmts = pkbFacade->getModifiesStmt();
	std::vector<int> expectedMStmts = {1, 2, 3, 4};
	REQUIRE(checkVecValuesEqual(mStmts, expectedMStmts));

	// getModifiesVariable
	std::vector<std::string> mVars = pkbFacade->getModifiesVariable();
	std::vector<std::string> expectedMVars = {"x", "y", "z", "a", "b", "c"};
	REQUIRE(checkVecValuesEqual(mVars, expectedMVars));

	// containsModifiesRelationship
	bool containsModifiesRelationship = pkbFacade->containsModifiesRelationship(4, "a");
	REQUIRE(containsModifiesRelationship == true);
  }

  SECTION("PKBFacade - UsesProcManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// hasUsesProcRelationship
	bool hasUsesProcRelationship = pkbFacade->hasUsesProcRelationship();
	REQUIRE(hasUsesProcRelationship == false);

	// insertUsesProc
	pkbFacade->insertUsesProc("main", "x");
	pkbFacade->insertUsesProc("main", "y");
	pkbFacade->insertUsesProc("main", "z");
	// insert Set
	std::unordered_set<std::string> variableSet = {"a", "b", "c"};
	pkbFacade->insertUsesProc("main", variableSet);

	// Query: Select v such that Uses("main", v)
	// getUsesProcVariable
	std::vector<std::string> variables = pkbFacade->getUsesProcVariable("main");
	std::vector<std::string> expectedValues = {"x", "y", "z", "a", "b", "c"};
	REQUIRE(checkVecValuesEqual(variables, expectedValues));

	// containsUsesProc
	bool containsUsesProc = pkbFacade->containsUsesProc("main");
	REQUIRE(containsUsesProc == true);

	// containsUsesProcVariable
	bool containsUsesProcVariable = pkbFacade->containsUsesProcVariable("a");
	REQUIRE(containsUsesProcVariable == true);

	// containsUsesProcRelationship
	bool containsUsesProcRelationship = pkbFacade->containsUsesProcRelationship("main", "a");
	REQUIRE(containsUsesProcRelationship == true);
  }

  SECTION("PKBFacade - ModifiesProcManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// hasModifiesProcRelationship
	bool hasModifiesProcRelationship = pkbFacade->hasModifiesProcRelationship();
	REQUIRE(hasModifiesProcRelationship == false);

	// insertModifiesProc
	pkbFacade->insertModifiesProc("modifiesFunction", "x");
	pkbFacade->insertModifiesProc("modifiesFunction", "y");
	pkbFacade->insertModifiesProc("modifiesFunction", "z");
	// insert Set
	std::unordered_set<std::string> variableSet = {"a", "b", "c"};
	pkbFacade->insertModifiesProc("modifiesFunction", variableSet);

	// Query: Select v such that Modifies("modifiesFunction", v)
	// getModifiesProcVariable
	std::vector<std::string> variables = pkbFacade->getModifiesProcVariable("modifiesFunction");
	std::vector<std::string> expectedValues = {"x", "y", "z", "a", "b", "c"};
	REQUIRE(checkVecValuesEqual(variables, expectedValues));

	// containsModifiesProc
	bool containsModifiesProc = pkbFacade->containsModifiesProc("modifiesFunction");
	REQUIRE(containsModifiesProc == true);

	// containsModifiesProcVariable
	bool containsModifiesProcVariable = pkbFacade->containsModifiesProcVariable("a");
	REQUIRE(containsModifiesProcVariable == true);

	// containsModifiesProcRelationship
	bool containsModifiesProcRelationship = pkbFacade->containsModifiesProcRelationship("modifiesFunction", "a");
	REQUIRE(containsModifiesProcRelationship == true);
  }

  SECTION("PKBFacade - CallsProcManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// hasCallsProcRelationship
	bool hasCallsProcRelationship = pkbFacade->hasCallsProcRelationship();
	REQUIRE(hasCallsProcRelationship == false);

	// Procudure : Procedure key value pair
	// insertCallsProc
	pkbFacade->insertCallsProc("main", "function1");
	pkbFacade->insertCallsProc("main", "function2");
	pkbFacade->insertCallsProc("function1", "function2");

	// Query: Select v such that Calls("main", v)
	// getCallsProcCallee, what it calls on
	std::vector<std::string> variables = pkbFacade->getCallsProcCallee("main");
	std::vector<std::string> expectedValues = {"function1", "function2"};
	REQUIRE(checkVecValuesEqual(variables, expectedValues));

	// containsCallsProcCaller
	bool containsCallsProc = pkbFacade->containsCallsProcCaller("main");
	REQUIRE(containsCallsProc == true);

	// containsCallsProcRelationship
	bool containsCallsProcRelationship = pkbFacade->containsCallsProcRelationship("main", "function1");
	REQUIRE(containsCallsProcRelationship == true);

	// hasCallsProcRelationship
	bool hasCallsProc = pkbFacade->hasCallsProcRelationship();
	REQUIRE(hasCallsProc == true);

	// containsCallsProcCallee
	bool containsCallsProcCallee = pkbFacade->containsCallsProcCallee("function3");
	REQUIRE(containsCallsProcCallee == false);
  }

  SECTION("PKBFacade - CallsTProcManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	auto pkbFacade = pkb->pkbFacade;

	// hasCallsTProcRelationship
	bool hasCallsTProcRelationship = pkbFacade->hasCallsTProcRelationship();
	REQUIRE(hasCallsTProcRelationship == false);

	// insertCallsTProc
	pkbFacade->insertCallsTProc("maint", "function1t");
	pkbFacade->insertCallsTProc("maint", "function2t");
	pkbFacade->insertCallsTProc("function1t", "function2t");

	// getCallsTProcCallee, Calls*("maint", p)
	std::vector<std::string> variables = pkbFacade->getCallsTProcCallee("maint");
	std::vector<std::string> expectedValues = {"function1t", "function2t"};
	REQUIRE(checkVecValuesEqual(variables, expectedValues));

	// containsCallsTProcCaller
	bool containsCallsTProc = pkbFacade->containsCallsTProcCaller("maint");
	REQUIRE(containsCallsTProc == true);

	// containsCallsTProcRelationship
	bool containsCallsTProcRelationship = pkbFacade->containsCallsTProcRelationship("maint", "function1t");
	REQUIRE(containsCallsTProcRelationship == true);

	// hasCallsTProcRelationship
	bool hasCallsTProc = pkbFacade->hasCallsTProcRelationship();
	REQUIRE(hasCallsTProc == true);

	// containsCallsTProcCallee
	bool containsCallsTProcCaller = pkbFacade->containsCallsTProcCaller("function3t");
	REQUIRE(containsCallsTProcCaller == false);
  }
}

