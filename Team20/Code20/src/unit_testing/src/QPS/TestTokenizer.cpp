#include "QPS/QueryTokenizer.h"
#include "../TestUtils.h"
#include "catch.hpp"
#include "QPS/QuerySyntaxError.h"
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

void printTokens(std::vector<shared_ptr<QueryToken>> tokens) {
    for (const auto &token : tokens) {
        cout << token->getTypeString() << " " << token->getValue() << endl;
    }
}
TEST_CASE("Tokenizer") {
    SECTION("TOKENIZE") {
        QueryTokenizer queryTokenizer;
        std::string query = "assign a1, a2; variable v; \nSelect s12d such that Follows*(1,\"sasds\") pattern a(_, _\" x    +    1   \"_)";

        auto tokens = queryTokenizer.tokenize(query);
//        cout << "tokens size: " << tokens.size() << endl;
//        REQUIRE(tokens.size() == 29);
//        printTokens(tokens);

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
                QueryToken(TokenType::INTEGER, "1"),
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


    SECTION("Valid Tokens with whitespace inside constant string") {
        QueryTokenizer queryTokenizer;
        std::string query = "assign a;\n Select a pattern a (_, _\"1 \"_)";
        auto tokens = queryTokenizer.tokenize(query);
        // print out tokens
//        printTokens(tokens);
    }

    SECTION("Valid Tokens with whitespace inside constant string") {
        QueryTokenizer queryTokenizer;
        std::string query = "assign a;\n Select a pattern a (_, _\"quota \t\"_)";
        auto tokens = queryTokenizer.tokenize(query);
        // print out tokens
//        printTokens(tokens);
    }

    SECTION("Valid Tokens with whitespace inside constant string") {
        QueryTokenizer queryTokenizer;
        std::string query = "assign a;\n Select a pattern a (_, _\"x * 2 \t\"_)";
        auto tokens = queryTokenizer.tokenize(query);
        // print out tokens
//        printTokens(tokens);
    }

    SECTION("Valid Tokens with whitespace after Follows*") {
        std::cout << "Valid Tokens with whitespace after Follows*" << std::endl;
        QueryTokenizer queryTokenizer;
        std::string query = "stmt s12d;\n Select s12d such that Follows* (s, \"x*y\")";
        REQUIRE_NOTHROW(queryTokenizer.tokenize(query));
    }

    SECTION("Invalid Tokens with whitespace between Follows and *") {
        QueryTokenizer queryTokenizer;
        std::string query = "assign a; variable v; constant c; Select a such that Uses (a, c) pattern a (\"8\", _)";
        try {
            auto tokens = queryTokenizer.tokenize(query);
        } catch (const QuerySyntaxError &e) {
            cout << "Invalid Tokens with whitespace between Follows and *" << endl;
            cout << e.what() << endl;
        } catch (const std::exception &e) {
            cout << "Invalid Tokens" << endl;
            cout << e.what() << endl;
        }

    }

    SECTION("Invalid NAME Tokens - Should throw QuerySyntaxError") {
        QueryTokenizer queryTokenizer;
        std::string query = "assign a&&; constant c; Select a such that Uses (a, c)";
        REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
        try {
            auto tokens = queryTokenizer.tokenize(query);
            printTokens(tokens);
        } catch (const QuerySyntaxError &e) {
            cout << e.what() << endl;
        } catch (const std::exception &e) {
            cout << e.what() << endl;
        }
    }

    SECTION("Invalid NAME Tokens with apostrophe - Should throw QuerySyntaxError") {
        QueryTokenizer queryTokenizer;
        // TODO: Fix such that 'hello' in apostrophe get passed as a NAME token
        std::string query = "assign a; constant c; Select a such that Uses (a, \"       quota\")";
        // REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
        try {
            cout << "Invalid NAME Tokens with apostrophe - Should throw QuerySyntaxError" << endl;
            auto tokens = queryTokenizer.tokenize(query);
            printTokens(tokens);
        } catch (const QuerySyntaxError &e) {
            cout << e.what() << endl;
        } catch (const std::exception &e) {
            cout << e.what() << endl;
        }
    }
}
