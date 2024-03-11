#include "PKB/manager/relationship/RelationshipManager.h"
#include "PKB/manager/relationship/FollowsManager.h"
#include "catch.hpp"
#include "PKB/Database.h"
#include "PKB/PKB.h"
#include "../TestUtils.h"


TEST_CASE("PKB Test") {
    // initialise database
    auto db = Database();

    SECTION("RelationshipManager") {
        std::cout << "Section: RelationshipManager" << std::endl;
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
        std::cout << "Section: FollowsManager" << std::endl;
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
        std::cout << "Section: FollowsManager from Database" << std::endl;
        // Create an instance of FollowsManager
        auto followsManager = db.getFollowsManager();
        // Insert some key-value pairs
        followsManager -> insert(1, 2);
        followsManager -> insert(2, 3);
        followsManager -> insert(3, 4);

        // Get values associated with a key: get
        std::vector<int> values = followsManager -> get(1);
        std::vector<int> expectedValues = {2};
        REQUIRE(checkVecValuesEqual(values, expectedValues));

        // Get keys associated with a value: getReverse
        std::vector<int> keys = followsManager -> getReverse(3);
        std::vector<int> expectedKeys = {2};
        REQUIRE(checkVecValuesEqual(keys, expectedKeys));

        // Check if a key exists: contains
        bool containsKey = followsManager -> contains(2);
        REQUIRE(containsKey == true);

        // Check if a value exists: containsReverse
        bool containsValue = followsManager -> containsReverse(4);
        REQUIRE(containsValue == true);
    }

        // interact with only IPKBWriter
    SECTION("PKB - SP - IPKBWriter - insertFollows - FollowsManager") {
        std::cout << "Section: SP - IPKBWriter - FollowsManager" << std::endl;
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
        std::cout << "Section: QPS - IPKBReader - FollowsManager" << std::endl;
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

    SECTION("PKB - PKBFacade - insertFollows - getFollows - containsFollows - containsFollowing - FollowsManager") {
        std::cout << "Section: PKBFacade - FollowsManager" << std::endl;
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

    SECTION("PKB - PKBFacade - insertFollowsT - getFollowsT - getFollowingT - containsFollowT - containsFollowingT - FollowsTManager") {
        std::cout << "Section: PKBFacade - FollowsTManager" << std::endl;
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
        std::cout << "\tFollower set: " <<  " ";
        for (const auto& element : values) {
            std::cout <<  element << " ";
        }
        std::cout << std::endl;

        // Query: select s such that Follows*(s, 3)
        // getFollowingT, 3 follows after 1, 2 transitively
        std::vector<int> valuesFollowingT = pkbFacade->getFollowingT(3);
        std::vector<int> expectedValuesFollowingT = {1, 2};

        REQUIRE(checkVecValuesEqual(valuesFollowingT, expectedValuesFollowingT));
        std::cout << "\tFollowing set: " <<  " ";
        for (const auto& element : valuesFollowingT) {
            std::cout << element << " ";
        }
        std::cout << std::endl;

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

    SECTION("PKB - PKBFacade - insertParent - getChild - getParent- containsParent - containsChild - ParentManager") {
        std::cout << "Section: PKBFacade - ParentManager" << std::endl;
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

        std::cout << "\tChild set: " <<  " ";
        for (const auto& element : childVec) {
            std::cout <<  element << " ";
        }
        std::cout << std::endl;

        // select s such that Parent(s, 3)
        // getParent
        std::vector<int> parentVec = pkbFacade->getParent(3);
        std::vector<int> expectedValuesFollowingT = {2};

        REQUIRE(checkVecValuesEqual(parentVec, expectedValuesFollowingT));

        std::cout << "\tParent set: " <<  " ";
        for (const auto& element : parentVec) {
            std::cout << element << " ";
        }
        std::cout << std::endl;

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

    SECTION("Test PKB - PKBFacade - insertVariable - getAllVariables - containsVariable - VariableManager") {
        std::cout << "Section: PKBFacade - VariableManager" << std::endl;
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

        std::cout << "\tVariable set: " <<  " ";
        for (const auto& element : values) {
            std::cout << element << " ";
        }
        std::cout << std::endl;

        // containsVariable
        bool containsVariableX = pkbFacade->containsVariable("x");
        REQUIRE(containsVariableX == true);

        bool containsVariableZ = pkbFacade->containsVariable("z");
        REQUIRE(containsVariableZ == false);

    }

    // int type: Assign, statement, while, if, call, print, read, constant
    SECTION("Test PKB - EntityManager<int>") {
        std::cout << "Section: EntityManager<int>" << std::endl;
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
    SECTION("Test PKB - EntityManager<string>") {
        std::cout << "Section: EntityManager<string>" << std::endl;
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
    }

    SECTION("Test PKB - PKBFacade - insertAssign - getAllAssignStmtNum - AssignManager") {
        std::cout << "Section: PKBFacade - AssignManager" << std::endl;
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

    SECTION("UsesStmt") {
        std::cout << "Section: PKBFacade - UsesStmtManager" << std::endl;
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

    SECTION("ModifiesStmt") {
        std::cout << "Section: PKBFacade - ModifiesStmtManager" << std::endl;
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

    SECTION("UsesProc") {
    std::cout << "Section: PKBFacade - UsesProcManager" << std::endl;
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

    SECTION("ModifiesProc") {
        std::cout << "Section: PKBFacade - ModifiesProcManager" << std::endl;
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
}


