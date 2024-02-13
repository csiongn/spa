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

TEST_CASE("Test TokenizeProgram") {
    // The testCases format is input_string : result_list
    // To add testcases, append to testCases list
    std::vector<TokenizerTestCase> testCases = {
        {
            "Hello\nWorld", { // Test Line Break
                Token(SP::TokenType::NAME, "Hello", 1),
                Token(SP::TokenType::NAME, "World", 2)
            },
        },
        {
            "flag = 0;", { // Test special character at end of string
                Token(SP::TokenType::NAME, "flag", 1),
                Token(SP::TokenType::EQUAL, "=", 1),
                Token(SP::TokenType::INTEGER, "0", 1),
                Token(SP::TokenType::SEMICOLON, ";", 1),
            },
        },
        {
            "call computeCentroid;", { // Test keywords - call
                Token(SP::TokenType::KEYWORD_CALL, "call", 1),
                Token(SP::TokenType::NAME, "computeCentroid", 1),
                Token(SP::TokenType::SEMICOLON, ";", 1),
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
            },
        },
        // You can add more test cases here
    };

    for (const auto& tc : testCases) {
        Tokenizer tokenizer(tc.input);
        tokenizer.tokenizeProgram();
        const std::vector<Token>& tokens = tokenizer.getTokens();
        CAPTURE(tokens, tc.expectedTokens);
        REQUIRE(tokens == tc.expectedTokens);
    }

}

//TEST_CASE("Test SP::TokenType") {
//    // Positive test cases
//    REQUIRE(SP::TokenType("MyVariable") == SP::TokenType::NAME);
//    REQUIRE(Token::SP::TokenType("123") == SP::TokenType::INTEGER);
//    REQUIRE(Token::SP::TokenType("123") == SP::TokenType::INTEGER);
//    REQUIRE(Token::SP::TokenType("=") == SP::TokenType::EQUAL);
//    REQUIRE(Token::SP::TokenType("+") == SP::TokenType::PLUS);
//    REQUIRE(Token::SP::TokenType("-") == SP::TokenType::MINUS);
//    REQUIRE(Token::SP::TokenType("*") == SP::TokenType::TIMES);
//    REQUIRE(Token::SP::TokenType("/") == SP::TokenType::SLASH);
//    REQUIRE(Token::SP::TokenType("%") == SP::TokenType::PERCENT);
//    REQUIRE(Token::SP::TokenType("&&") == SP::TokenType::AND);
//    REQUIRE(Token::SP::TokenType("||") == SP::TokenType::OR);
//    REQUIRE(Token::SP::TokenType("!") == SP::TokenType::NOT);
//    REQUIRE(Token::SP::TokenType("<") == SP::TokenType::LESS_THAN);
//    REQUIRE(Token::SP::TokenType("<=") == SP::TokenType::LESS_THAN_EQUAL);
//    REQUIRE(Token::SP::TokenType(">") == SP::TokenType::GREATER_THAN);
//    REQUIRE(Token::SP::TokenType(">=") == SP::TokenType::GREATER_THAN_EQUAL);
//    REQUIRE(Token::SP::TokenType("==") == SP::TokenType::DOUBLE_EQUAL);
//    REQUIRE(Token::SP::TokenType("(") == SP::TokenType::LEFT_PAREN);
//    REQUIRE(Token::SP::TokenType(")") == SP::TokenType::RIGHT_PAREN);
//    REQUIRE(Token::SP::TokenType("{") == SP::TokenType::LEFT_BRACE);
//    REQUIRE(Token::SP::TokenType("}") == SP::TokenType::RIGHT_BRACE);
//    REQUIRE(Token::SP::TokenType(";") == SP::TokenType::SEMICOLON);
//    REQUIRE(Token::SP::TokenType("procedure") == SP::TokenType::KEYWORD_PROCEDURE);
//    REQUIRE(Token::SP::TokenType("call") == SP::TokenType::KEYWORD_CALL);
//    REQUIRE(Token::SP::TokenType("print") == SP::TokenType::KEYWORD_PRINT);
//    REQUIRE(Token::SP::TokenType("read") == SP::TokenType::KEYWORD_READ);
//    REQUIRE(Token::SP::TokenType("while") == SP::TokenType::KEYWORD_WHILE);
//    REQUIRE(Token::SP::TokenType("if") == SP::TokenType::KEYWORD_IF);
//    REQUIRE(Token::SP::TokenType("then") == SP::TokenType::KEYWORD_THEN);
//    REQUIRE(Token::SP::TokenType("else") == SP::TokenType::KEYWORD_ELSE);
//
//    // Negative test cases
//    REQUIRE(Token::SP::TokenType("procedures") != SP::TokenType::KEYWORD_PROCEDURE); // keyword spelling is different
//    REQUIRE(Token::SP::TokenType("Procedures") != SP::TokenType::KEYWORD_PROCEDURE); // case-sensitivity
//
//    // Alphanumeric strings
//    REQUIRE(Token::SP::TokenType("procedures123") == SP::TokenType::NAME);
//
//}

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


