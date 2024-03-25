#include <SP/CallGraph.h>

#include "../lib/catch.hpp"
#include "PKB/PKB.h"
#include "PKB/facade/PKBFacade.h"

TEST_CASE("Add call relationships and finalize CallGraph") {

    GIVEN("A call graph") {
        CallGraph cg;

        WHEN("Some relationships are added and finalized") {
            cg.addCall("main", "A");
            cg.addCall("A", "B");
            cg.addCall("A", "C");
            cg.addCall("D", "E");
            cg.addCall("E", "procH");
            cg.finalize();

            THEN("The correct call relationships should be added") {
                REQUIRE(cg.hasCallsRelationship("main", "A"));
                REQUIRE(cg.hasCallsRelationship("A", "B"));
                REQUIRE(cg.hasCallsRelationship("A", "C"));
                REQUIRE_FALSE(cg.hasCallsRelationship("proc123", "C")); // Proc that doesn't exist
            }

            THEN("The correct callsT relationships should be computed") {
                REQUIRE(cg.hasCallsTRelationship("main", "B"));
                REQUIRE(cg.hasCallsTRelationship("main", "C"));
                REQUIRE(cg.hasCallsTRelationship("A", "C"));
                REQUIRE(cg.hasCallsTRelationship("D", "procH"));
                REQUIRE_FALSE(cg.hasCallsTRelationship("B", "A")); // Non-existing path
                REQUIRE_FALSE(cg.hasCallsTRelationship("proc123", "A")); // Proc that doesn't exist
            }

        }
    }

    GIVEN("A finalized call graph") {
        CallGraph cg;
        cg.addCall("main", "A");
        cg.finalize();

        THEN("Adding more relationships should throw an error") {
            REQUIRE_THROWS_AS(cg.addCall("B", "C"), std::runtime_error);
        }
    }

    GIVEN("A call graph with no function calls") {
        CallGraph cg;
        THEN("Finalizing it should work") {
            REQUIRE_NOTHROW(cg.finalize());
        }
    }
}
