#include "QPS/QueryTokenizer.h"
#include "catch.hpp"
using namespace std;

TEST_CASE("Tokenizer") {
    SECTION("TOKENIZE") {
        QueryTokenizer queryTokenizer;
        std::string query = "assign a; variable v; \nSelect s12d such that Follows*(1,\"sasds\") pattern a(_, \"v\")";
        // Doesnt stop the tokenization from '&&' within Follows because does not throw error now
//         std::string query = "Select s12d such that pattern a(_, v) 12Follows(1,\"x*y/3\");";

//         std::string query = "Select 1s12d such that pattern a(_, v) 123Follows(1,\"x*y/3\");";
//         std::string query = "Select 1s12d such that pattern a(_, v) Follows(1,\"x*y/3\");";
        // Cant check if invalid token is detected
        // std::string query = "Select s12d such that Follows(_,\"sas/*ds\")";

        auto tokens = queryTokenizer.tokenize(query);
        cout << "tokens size: " << tokens.size() << endl;
        for (auto token : tokens) {
            token->print();
        }
        REQUIRE(tokens.size() == 24);

    }
}