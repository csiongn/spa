#include "PKB/manager/relationship/RelationshipManager.h"
#include "PKB/manager/relationship/FollowsManager.h"
#include "catch.hpp"
#include "PKB/Database.h"
#include "PKB/PKB.h"


TEST_CASE("PKB Test") {
    // initialise database
    auto db = Database();
    auto followsManager = db.getFollowsManager();

    SECTION("RelationshipManager") {
        // Create an instance of RelationshipManager
        auto repo = RelationshipManager<int, std::string>();

        // Insert some key-value pairs
        repo.insert(1, "apple");
        repo.insert(2, "banana");
        repo.insert(1, "orange");
        repo.insert(3, "apple");

        // Get values associated with a key
        std::vector<std::string> values = repo.get(1);
        std::vector<std::string> expectedValues = {"apple", "orange"};
        REQUIRE(values == expectedValues);

        // Get keys associated with a value
        std::vector<int> keys = repo.getReverse("apple");
        std::vector<int> expectedKeys = {1, 3};
        REQUIRE(keys == expectedKeys);

        // Check if a key exists
        bool containsKey = repo.contains(2);
        REQUIRE(containsKey == true);

        // Check if a value exists
        bool containsValue = repo.containsReverse("banana");
        REQUIRE(containsValue == true);
    }

    SECTION("FollowsManager") {
        // Create an instance of FollowsManager
        auto repo = FollowsManager();

        // Insert some key-value pairs
        repo.insert(1, 2);
        repo.insert(2, 3);
        repo.insert(3, 4);

        // Get values associated with a key
        std::vector<int> values = repo.get(1);
        std::vector<int> expectedValues = {2};
        REQUIRE(values == expectedValues);

        // Get keys associated with a value
        std::vector<int> keys = repo.getReverse(3);
        std::vector<int> expectedKeys = {2};
        REQUIRE(keys == expectedKeys);

        // Check if a key exists
        bool containsKey = repo.contains(2);
        REQUIRE(containsKey == true);

        // Check if a value exists
        bool containsValue = repo.containsReverse(4);
        REQUIRE(containsValue == true);
    }

    SECTION("FollowsManager from Database") {
        // Create an instance of FollowsManager

        // Insert some key-value pairs
        followsManager -> insert(1, 2);
        followsManager -> insert(2, 3);
        followsManager -> insert(3, 4);

        // Get values associated with a key
        std::vector<int> values = followsManager -> get(1);
        std::vector<int> expectedValues = {2};
        REQUIRE(values == expectedValues);

        // Get keys associated with a value
        std::vector<int> keys = followsManager -> getReverse(3);
        std::vector<int> expectedKeys = {2};
        REQUIRE(keys == expectedKeys);

        // Check if a key exists
        bool containsKey = followsManager -> contains(2);
        REQUIRE(containsKey == true);

        // Check if a value exists
        bool containsValue = followsManager -> containsReverse(4);
        REQUIRE(containsValue == true);
    }

    SECTION("PKB - PKBFacade") {
        // Create an instance of PKB
        auto pkb = std::make_shared<PKB>();

        // Insert some key-value pairs
        pkb->pkbFacade->insertFollows(1, 2);
        pkb->pkbFacade->insertFollows(2, 3);

        // Get values associated with a key
        std::vector<int> values = pkb->pkbFacade->getFollows(1);
        std::vector<int> expectedValues = {2};
        REQUIRE(values == expectedValues);
    }

    // interact with only IPKBWriter
    SECTION("PKB - IPKBWriter") {
        // Create an instance of PKB
        auto pkb = std::make_shared<PKB>();
        std::shared_ptr<IPKBWriter> pkbWriter = pkb->pkbFacade;
        // Insert some key-value pairs
        pkbWriter->insertFollows(1, 2);
        pkbWriter->insertFollows(2, 3);

        // CATCH_ERROR(pkbWriter->getFollows(1));
        // Get values associated with a key
        std::vector<int> values = pkb->pkbFacade->getFollows(1);
        std::vector<int> expectedValues = {2};
        REQUIRE(values == expectedValues);
    }


    // interact with only IPKBReader
    SECTION("PKB - IPKBReader in QPS") {
        // Create an instance of PKB
        auto pkb = std::make_shared<PKB>();

        // QPS will receive a shared pointer to the PKB facade and initialize it to IPKBReader
        std::shared_ptr<IPKBReader> pkbReader = pkb->pkbFacade;
        // Insert some key-value pairs
        pkb->pkbFacade->insertFollows(1, 2);
        pkb->pkbFacade->insertFollows(2, 3);

        // CATCH_ERROR(pkbWriter->getFollows(1));
        // Get values associated with a key
        std::vector<int> values = pkbReader->getFollows(1);
        std::vector<int> expectedValues = {2};
        REQUIRE(values == expectedValues);
    }
}


