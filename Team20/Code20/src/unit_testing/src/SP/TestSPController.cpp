#include <unordered_set>
#include <unordered_map>

#include "catch.hpp"

#include "Models/SimpleProgram.h"
#include "PKB/Database.h"
#include "PKB/PKB.h"
#include "SP/SPController.h"
#include "../TestUtils.h"


TEST_CASE("SPController Test") {
    // initialise SPController
    auto pkb = std::make_shared<PKB>();
    auto pkbFacade = pkb->pkbFacade;
    // no input for SPController
    auto spController = SPController(pkbFacade, "");
    std::unordered_map<int, std::unordered_set<int>> follows = {
            {1, {2}},
            {2, {3}},
    };

    std::unordered_map<int, std::unordered_set<int>> followsT = {
            {1, {2, 3, 4, 5, 6, 7}},
            {2, {3, 4, 5, 6, 7, 8}},
    };

    std::unordered_map<int, std::unordered_set<int>> parent = {
            {3, {4}},
            {4, {5}},
    };

    std::unordered_map<int, std::unordered_set<int>> parentT = {
            {3, {4, 5, 6, 7, 8, 9}},
            {4, {5, 6, 7, 8, 9, 10}},
    };

    // Entities
    std::unordered_set<int> stmtSet = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::unordered_set<std::string> varSet = {"x", "y", "z"};

    SECTION("Test insertion of relationship") {
        // set of relationships for follows, followsT, parents, parentT
        spController.insertRelationship(SimpleProgram::DesignAbstraction::FOLLOWS, follows);
        spController.insertRelationship(SimpleProgram::DesignAbstraction::FOLLOWST, followsT);
        spController.insertRelationship(SimpleProgram::DesignAbstraction::PARENT, parent);
        spController.insertRelationship(SimpleProgram::DesignAbstraction::PARENTT, parentT);

        // check if the relationships are inserted correctly
        // use pkbFacade as reader to verify results
        std::vector<int> expectedFollowerValues = {2};
        std::vector<int> expectedFollowerTValues = {2, 3, 4, 5, 6, 7};
        std::vector<int> expectedChildValues = {4};
        std::vector<int> expectedChildTValues = {4, 5, 6, 7, 8, 9};
        REQUIRE(checkVecValuesEqual(pkbFacade->getFollows(1), expectedFollowerValues));
        REQUIRE(checkVecValuesEqual(pkbFacade->getFollowsT(1), expectedFollowerTValues));
        REQUIRE(checkVecValuesEqual(pkbFacade->getChild(3), expectedChildValues));
        REQUIRE(checkVecValuesEqual(pkbFacade->getChildT(3), expectedChildTValues));
    }

    SECTION("Test insertion of entity") {
        spController.insertEntity(SimpleProgram::DesignEntity::STMT, stmtSet);
        spController.insertEntity(SimpleProgram::DesignEntity::VARIABLE, varSet);

        std::vector<int> expectedStmtValue = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<std::string> expectedVarValue = {"x", "y", "z"};

        REQUIRE(checkVecValuesEqual(pkbFacade->getAllStatementNum(), expectedStmtValue));
        REQUIRE(checkVecValuesEqual(pkbFacade->getAllVariables(), expectedVarValue));
    }

    SECTION("Test insertion of pattern") {
        // TODO: Add test cases for pattern
    }
}
