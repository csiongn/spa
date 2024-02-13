#include "catch.hpp"
#include "QPS/QueryParser.h"
#include "QPS/QueryTokenizer.h"
using namespace std;

TEST_CASE("QueryParser") {
    SECTION("PARSE") {
        QueryTokenizer queryTokenizer;
        std::string query = "variable v; \nSelect v";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }
}


