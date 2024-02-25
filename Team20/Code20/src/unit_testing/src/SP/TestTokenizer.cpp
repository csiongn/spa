#include "SP/TNode.h"
#include "SP/Tokenizer.h"
#include "catch.hpp"
#include <iterator>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

struct TokenizerTestCase {
    std::string input;
    std::vector<Token> expectedTokens;
};

struct TokenWordTestCase {
    std::string input;
    std::vector<std::string> expectedTokens;
};

std::string tokenToString(const Token& token) {
    std::stringstream ss;
    ss << "\nType: " << static_cast<int>(token.type) << ", Value: " << token.value << ", Line: " << token.line_num;
    return ss.str();
}

std::string tokensToString(const std::vector<Token>& tokens) {
    std::stringstream ss;
    for (const auto& token : tokens) {
        ss << tokenToString(token);
    }
    return ss.str();
}

TEST_CASE("Test TokenizeProgram") {
    // The testCases format is input_string : result_list
    // To add testcases, append to testCases list
    std::vector<TokenizerTestCase> testCases = {
        {
            "Hello\nWorld", { // Test Line Break
                Token(SP::TokenType::NAME, "Hello", 1),
                Token(SP::TokenType::NAME, "World", 2),
                Token(SP::TokenType::EOFILE, "", 2)
            },
        },
        {
            "flag = 0;", { // Test special character at end of string
                Token(SP::TokenType::NAME, "flag", 1),
                Token(SP::TokenType::EQUAL, "=", 1),
                Token(SP::TokenType::INTEGER, "0", 1),
                Token(SP::TokenType::SEMICOLON, ";", 1),
                Token(SP::TokenType::EOFILE, "", 2)
            },
        },
        {
            "call computeCentroid;", { // Test keywords - call
                Token(SP::TokenType::KEYWORD_CALL, "call", 1),
                Token(SP::TokenType::NAME, "computeCentroid", 1),
                Token(SP::TokenType::SEMICOLON, ";", 1),
                Token(SP::TokenType::EOFILE, "", 2)
            },

        },
        {
            "while (x > 0) {\n x = x + 1 \n }""", { // Test keywords - while
                Token(SP::TokenType::KEYWORD_WHILE, "while", 1),
                Token(SP::TokenType::LEFT_PAREN, "(", 1),
                Token(SP::TokenType::NAME, "x", 1),
                Token(SP::TokenType::GREATER_THAN, ">", 1),
                Token(SP::TokenType::INTEGER, "0", 1),
                Token(SP::TokenType::RIGHT_PAREN, ")", 1),
                Token(SP::TokenType::LEFT_BRACE, "{", 1),
                Token(SP::TokenType::NAME, "x", 2),
                Token(SP::TokenType::EQUAL, "=", 2),
                Token(SP::TokenType::NAME, "x", 2),
                Token(SP::TokenType::PLUS, "+", 2),
                Token(SP::TokenType::INTEGER, "1", 2),
                Token(SP::TokenType::RIGHT_BRACE, "}", 3),
                Token(SP::TokenType::EOFILE, "", 2)
            },
        },
        {
            "x == 3;", { // Test double symbol - found
                Token(SP::TokenType::NAME, "x", 1),
                Token(SP::TokenType::DOUBLE_EQUAL, "==", 1),
                Token(SP::TokenType::INTEGER, "3", 1),
                Token(SP::TokenType::SEMICOLON, ";", 1),
                Token(SP::TokenType::EOFILE, "", 2)
            },
        },
        {
            "x <= 3;", { // Test <= symbol tokenizes into one
                Token(SP::TokenType::NAME, "x", 1),
                Token(SP::TokenType::LESS_THAN_EQUAL, "<=", 1),
                Token(SP::TokenType::INTEGER, "3", 1),
                Token(SP::TokenType::SEMICOLON, ";", 1),
                Token(SP::TokenType::EOFILE, "", 2)
            },
        },
        
        // You can add more test cases here
    };

    for (const auto& tc : testCases) {
        Tokenizer tokenizer(tc.input);
        tokenizer.tokenizeProgram();
        const std::vector<Token>& tokens = tokenizer.getTokens();
        CAPTURE(tokensToString(tokens), tokensToString(tc.expectedTokens));

        REQUIRE(tokens == tc.expectedTokens);
    }
}

TEST_CASE("Test exception throwing in tokenizeWord") {
    std::vector<std::string> error_programs =
    {
        "x /= 3;", // invalid symbol - length 2
        "x /== 3;", // invalid symbol - length 3
        "x ??? 3;", // invalid symbol - unsupported symbols
        "123nums = 3;", // invalid variable name
        "x = 3;;", // invalid symbol at EOL
        "x <== 3", // invalid symbol - substrings of symbols are valid but whole string is not
        "x<==3;", // invalid symbol - no spaces
    };
    for (const auto& error_program : error_programs) {
        Tokenizer tokenizer(error_program);
        REQUIRE_THROWS(tokenizer.tokenizeProgram());
    }
}

TEST_CASE("Test tokenizeWord") {
    // The testCases format is input_string : result_list
    // To add testcases, append to testCases list
    std::vector<TokenWordTestCase> testCases = {
        {
            "Hello;World", { // Test single symbol inside
                "Hello",
                ";",
                "World",
            },
        },
        {
            "x==2;", { // Test double symbol inside
                "x",
                "==",
                "2",
                ";",
            },
        },
        {
            "x===2;", { // Test triple symbol inside
                "x",
                "===",
                "2",
                ";",
            },
        },
        {
            "readX;readY;", { // Test keyword parsing
                "readX",
                ";",
                "readY",
                ";",
            },
        },
        // You can add more test cases here
    };

    for (const auto& tc : testCases) {
        Tokenizer tokenizer(tc.input);
        const std::vector<std::string>& tokens = tokenizer.tokenizeWord(tc.input);
        CAPTURE(tokens, tc.expectedTokens);
        REQUIRE(tokens == tc.expectedTokens);
    }

}


TEST_CASE("Test tokenizeLine") {
    // The testCases format is input_string : result_list
    // To add testcases, append to testCases list
    std::vector<TokenizerTestCase> testCases = {
        {
            "Hello;World;", { // Test single symbol inside
                Token(SP::TokenType::NAME, "Hello", 1),
                Token(SP::TokenType::SEMICOLON, ";", 1),
                Token(SP::TokenType::NAME, "World", 1),
                Token(SP::TokenType::SEMICOLON, ";", 1),
            },
        },
        {
            "while((x==2)&&(y==2))", { // Test parenthesis tokenizing
                Token(SP::TokenType::KEYWORD_WHILE, "while", 1),
                Token(SP::TokenType::LEFT_PAREN, "(", 1),
                Token(SP::TokenType::LEFT_PAREN, "(", 1),
                Token(SP::TokenType::NAME, "x", 1),
                Token(SP::TokenType::DOUBLE_EQUAL, "==", 1),
                Token(SP::TokenType::INTEGER, "2", 1),
                Token(SP::TokenType::RIGHT_PAREN, ")", 1),
                Token(SP::TokenType::AND, "&&", 1),
                Token(SP::TokenType::LEFT_PAREN, "(", 1),
                Token(SP::TokenType::NAME, "y", 1),
                Token(SP::TokenType::DOUBLE_EQUAL, "==", 1),
                Token(SP::TokenType::INTEGER, "2", 1),
                Token(SP::TokenType::RIGHT_PAREN, ")", 1),
                Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            },
        },
        {
            "while((x<=2)&&(y<2))", { // Test correct tokenizing of <= and <
                Token(SP::TokenType::KEYWORD_WHILE, "while", 1),
                Token(SP::TokenType::LEFT_PAREN, "(", 1),
                Token(SP::TokenType::LEFT_PAREN, "(", 1),
                Token(SP::TokenType::NAME, "x", 1),
                Token(SP::TokenType::LESS_THAN_EQUAL, "<=", 1),
                Token(SP::TokenType::INTEGER, "2", 1),
                Token(SP::TokenType::RIGHT_PAREN, ")", 1),
                Token(SP::TokenType::AND, "&&", 1),
                Token(SP::TokenType::LEFT_PAREN, "(", 1),
                Token(SP::TokenType::NAME, "y", 1),
                Token(SP::TokenType::LESS_THAN, "<", 1),
                Token(SP::TokenType::INTEGER, "2", 1),
                Token(SP::TokenType::RIGHT_PAREN, ")", 1),
                Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            },
        },
        {
            "while(((x==2)&&(y==2))&&(while((a==2) && (v==2)))){", { // Test parenthesis - limit
            Token(SP::TokenType::KEYWORD_WHILE, "while", 1),
            Token(SP::TokenType::LEFT_PAREN, "(", 1),
            Token(SP::TokenType::LEFT_PAREN, "(", 1),
            Token(SP::TokenType::LEFT_PAREN, "(", 1),
            Token(SP::TokenType::NAME, "x", 1),
            Token(SP::TokenType::DOUBLE_EQUAL, "==", 1),
            Token(SP::TokenType::INTEGER, "2", 1),
            Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            Token(SP::TokenType::AND, "&&", 1),
            Token(SP::TokenType::LEFT_PAREN, "(", 1),
            Token(SP::TokenType::NAME, "y", 1),
            Token(SP::TokenType::DOUBLE_EQUAL, "==", 1),
            Token(SP::TokenType::INTEGER, "2", 1),
            Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            Token(SP::TokenType::AND, "&&", 1),
            Token(SP::TokenType::LEFT_PAREN, "(", 1),
            Token(SP::TokenType::KEYWORD_WHILE, "while", 1),
            Token(SP::TokenType::LEFT_PAREN, "(", 1),
            Token(SP::TokenType::LEFT_PAREN, "(", 1),
            Token(SP::TokenType::NAME, "a", 1),
            Token(SP::TokenType::DOUBLE_EQUAL, "==", 1),
            Token(SP::TokenType::INTEGER, "2", 1),
            Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            Token(SP::TokenType::AND, "&&", 1),
            Token(SP::TokenType::LEFT_PAREN, "(", 1),
            Token(SP::TokenType::NAME, "v", 1),
            Token(SP::TokenType::DOUBLE_EQUAL, "==", 1),
            Token(SP::TokenType::INTEGER, "2", 1),
            Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            Token(SP::TokenType::RIGHT_PAREN, ")", 1),
            Token(SP::TokenType::LEFT_BRACE, "{", 1),
            },
        },
        // You can add more test cases here
    };

    for (const auto& tc : testCases) {
        Tokenizer tokenizer(tc.input);
        std::istringstream line_stream(tc.input);
        const std::vector<Token>& tokens = tokenizer.tokenizeLineStream(line_stream, 1);
        CAPTURE(tokensToString(tokens), tokensToString(tc.expectedTokens));
        REQUIRE(tokens == tc.expectedTokens);
    }

}

