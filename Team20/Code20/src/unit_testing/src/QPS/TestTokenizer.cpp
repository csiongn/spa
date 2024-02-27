#include "QPS/QueryTokenizer.h"
#include "../TestUtils.h"
#include "catch.hpp"
using namespace std;

bool checkTokenVectorEqual(std::vector<shared_ptr<QueryToken>> vec1, std::vector<QueryToken> vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }
    for (int i = 0; i < vec1.size(); i++) {
        REQUIRE(vec1[i]->getTypeString() == vec2[i].getTypeString());
        REQUIRE(vec1[i]->getValue() == vec2[i].getValue());
    }
    for (int i = 0; i < vec1.size(); i++) {
        if (vec1[i]->getTypeString() != vec2[i].getTypeString() || vec1[i]->getValue() != vec2[i].getValue()) {
            return false;
        }
    }
    return true;
}

TEST_CASE("Tokenizer") {
    SECTION("TOKENIZE") {
        QueryTokenizer queryTokenizer;
        std::string query = "assign a1, a2; variable v; \nSelect s12d such that Follows*(1,\"sasds\") pattern a(_, _\" x    +    1   \"_)";

        auto tokens = queryTokenizer.tokenize(query);
        cout << "tokens size: " << tokens.size() << endl;
        REQUIRE(tokens.size() == 29);

        // Compare tokens
        std::vector<QueryToken> expectedTokensVector = {
                QueryToken(TokenType::NAME, "assign"),
                QueryToken(TokenType::NAME, "a1"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ","),
                QueryToken(TokenType::NAME, "a2"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ";"),
                QueryToken(TokenType::NAME, "variable"),
                QueryToken(TokenType::NAME, "v"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ";"),
                QueryToken(TokenType::NAME, "Select"),
                QueryToken(TokenType::NAME, "s12d"),
                QueryToken(TokenType::NAME, "such"),
                QueryToken(TokenType::NAME, "that"),
                QueryToken(TokenType::NAME, "Follows*"),
                QueryToken(TokenType::SPECIAL_CHARACTER, "("),
                QueryToken(TokenType::INTEGER, "1"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ","),
                QueryToken(TokenType::CONSTANT_STRING, "sasds"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ")"),
                QueryToken(TokenType::NAME, "pattern"),
                QueryToken(TokenType::NAME, "a"),
                QueryToken(TokenType::SPECIAL_CHARACTER, "("),
                QueryToken(TokenType::WILDCARD, "_"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ","),
                QueryToken(TokenType::WILDCARD, "_"),
                QueryToken(TokenType::NAME, "x"),
                QueryToken(TokenType::SPECIAL_CHARACTER, "+"),
                QueryToken(TokenType::NAME, "1"),
                QueryToken(TokenType::WILDCARD, "_"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ")")
        };

        // check each vector has to had the same typestring and value
        REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
    }

    SECTION("Valid Tokens") {
        QueryTokenizer queryTokenizer;
        std::string query = "Select s12d such that Follows*(s, \"x*y\")";
        auto tokens = queryTokenizer.tokenize(query);
        std::vector<QueryToken> expectTokensVector = {
                QueryToken(TokenType::NAME, "Select"),
                QueryToken(TokenType::NAME, "s12d"),
                QueryToken(TokenType::NAME, "such"),
                QueryToken(TokenType::NAME, "that"),
                QueryToken(TokenType::NAME, "Follows*"),
                QueryToken(TokenType::SPECIAL_CHARACTER, "("),
                QueryToken(TokenType::NAME, "s"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ","),
                QueryToken(TokenType::NAME, "x"),
                QueryToken(TokenType::SPECIAL_CHARACTER, "*"),
                QueryToken(TokenType::NAME, "y"),
                QueryToken(TokenType::SPECIAL_CHARACTER, ")")
        };
        // check each vector has to had the same typestring and value
        REQUIRE(checkTokenVectorEqual(tokens, expectTokensVector));
    }

    SECTION("Invalid Tokens in MS1 and Synonym used more than once") {
        QueryTokenizer queryTokenizer;
        std::string query = "procedure p, q;\nSelect BOOLEAN such that Calls(p, q) with q.procName = \"p\" and p.procName = \"Example\"";

        REQUIRE_THROWS(queryTokenizer.tokenize(query));
    }
}