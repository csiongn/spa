#include "PKB/manager/relationship/RelationshipManager.h"
#include "PKB/manager/relationship/FollowsManager.h"
#include "../lib/catch.hpp"
#include "PKB/Database.h"
#include "PKB/PKB.h"
#include "../TestUtils.h"

TEST_CASE("PKB Test") {
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

	// int type: Assign, statement, while, if, call, print, read, constant
  SECTION("EntityManager<int>") {
	// Create an instance of PKB
	auto intEntityManager = std::make_shared<EntityManager<int>>();

	// insert
	intEntityManager->insert(1);
	intEntityManager->insert(2);
	intEntityManager->insert(3);
	// insert duplicate
	intEntityManager->insert(1);

	// a can be assign, statement, while, if, call, print, read
	// Query: Select a
	// get
	auto values = intEntityManager->get();
	std::vector<int> expectedValues = {1, 2, 3};
	REQUIRE(checkVecValuesEqual(values, expectedValues));
	REQUIRE(intEntityManager->contains(1) == true);
  }

	// string type: procedure, variable
  SECTION("EntityManager<string>") {
	// Create an instance of PKB
	auto stringEntityManager = std::make_shared<EntityManager<std::string>>();
	auto procedureManager = std::make_shared<ProcedureManager>();

	// insert
	stringEntityManager->insert("main");
	stringEntityManager->insert("function1");
	stringEntityManager->insert("function2");

	procedureManager->insert("main");
	procedureManager->insert("function1");
	procedureManager->insert("function2");

	// insert duplicate
	stringEntityManager->insert("main");

	// a can be procedure, variable
	// Query: Select a
	// get
	auto stringEntityVec = stringEntityManager->get();
	auto procedureVec = procedureManager->get();
	REQUIRE(checkVecValuesEqual(stringEntityVec, procedureVec));
	REQUIRE(stringEntityManager->contains("main") == true);
	REQUIRE(procedureManager->contains("hello") == false);
  }SECTION("FollowsManager") {
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

  SECTION("FollowsManager from Database") {
	// Create an instance of FollowsManager
	auto followsManager = db.getFollowsManager();
	// Insert some key-value pairs
	followsManager->insert(1, 2);
	followsManager->insert(2, 3);
	followsManager->insert(3, 4);

	// Get values associated with a key: get
	std::vector<int> values = followsManager->get(1);
	std::vector<int> expectedValues = {2};
	REQUIRE(checkVecValuesEqual(values, expectedValues));

	// Get keys associated with a value: getReverse
	std::vector<int> keys = followsManager->getReverse(3);
	std::vector<int> expectedKeys = {2};
	REQUIRE(checkVecValuesEqual(keys, expectedKeys));

	// Check if a key exists: contains
	bool containsKey = followsManager->contains(2);
	REQUIRE(containsKey == true);

	// Check if a value exists: containsReverse
	bool containsValue = followsManager->containsReverse(4);
	REQUIRE(containsValue == true);
  }

	// interact with only IPKBWriter
  SECTION("PKB - SP - IPKBWriter - insertFollows - FollowsManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();
	std::shared_ptr<IPKBWriter> pkbWriter = pkb->pkbFacade;
	// hasFollowsRelationship
	bool hasFollowsRelationship = pkb->pkbFacade->hasFollowsRelationship();
	REQUIRE(hasFollowsRelationship == false);

	// insertFollows
	pkbWriter->insertFollows(1, 2);
	pkbWriter->insertFollows(2, 3);

	// insertFollows with set
	std::unordered_set<int> followerSet = {4, 5, 6};
	pkbWriter->insertFollows(3, followerSet);

	// getFollows
	std::vector<int> values = pkb->pkbFacade->getFollows(1);
	std::vector<int> expectedValues = {2};
	REQUIRE(checkVecValuesEqual(values, expectedValues));

	// getFollows after insertion of set
	std::vector<int> valuesSet = pkb->pkbFacade->getFollows(3);
	std::vector<int> expectedValuesSet = {4, 5, 6};
	REQUIRE(checkVecValuesEqual(valuesSet, expectedValuesSet));

	// getFolloweeStmts
	std::vector<int> followeeStmts = pkb->pkbFacade->getFolloweeStmts();
	std::vector<int> expectedFolloweeStmts = {1, 2, 3};
	REQUIRE(checkVecValuesEqual(followeeStmts, expectedFolloweeStmts));

	// getFollowerStmts
	std::vector<int> followerStmts = pkb->pkbFacade->getFollowerStmts();
	std::vector<int> expectedFollowerStmts = {2, 3, 4, 5, 6};
	REQUIRE(checkVecValuesEqual(followerStmts, expectedFollowerStmts));
  }

	// interact with only IPKBReader
  SECTION("PKB - IPKBReader in QPS - getFollows - FollowsManager") {
	// Create an instance of PKB
	auto pkb = std::make_shared<PKB>();

	// Populate Follows Table for IPKBReader
	pkb->pkbFacade->insertFollows(1, 2);
	pkb->pkbFacade->insertFollows(2, 3);

	// QPS will receive a shared pointer to the PKB facade and initialize it to IPKBReader
	std::shared_ptr<IPKBReader> pkbReader = pkb->pkbFacade;
	// hasFollowsRelationship
	bool hasFollowsRelationship = pkbReader->hasFollowsRelationship();
	REQUIRE(hasFollowsRelationship == true);

	// Get values associated with a key
	std::vector<int> values = pkbReader->getFollows(1);
	std::vector<int> expectedValues = {2};
	REQUIRE(checkVecValuesEqual(values, expectedValues));
  }
}


