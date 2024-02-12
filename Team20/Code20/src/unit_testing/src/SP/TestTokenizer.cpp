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
                Token(TokenType::NAME, "Hello", 1),
                Token(TokenType::NAME, "World", 2)
            },
        },
        {
            "flag = 0;", { // Test special character at end of string
                Token(TokenType::NAME, "flag", 1),
                Token(TokenType::EQUAL, "=", 1),
                Token(TokenType::INTEGER, "0", 1),
                Token(TokenType::SEMICOLON, ";", 1),
            },
        },
        {
            "call computeCentroid;", { // Test keywords - call
                Token(TokenType::KEYWORD_CALL, "call", 1),
                Token(TokenType::NAME, "computeCentroid", 1),
                Token(TokenType::SEMICOLON, ";", 1),
            },
            
        },
        {
            "while (x > 0) {\n x = x + 1 \n }""", { // Test keywords - while
                Token(TokenType::KEYWORD_WHILE, "while", 1),
                Token(TokenType::LEFT_PAREN, "(", 1),
                Token(TokenType::NAME, "x", 1),
                Token(TokenType::GREATER_THAN, ">", 1),
                Token(TokenType::INTEGER, "0", 1),
                Token(TokenType::RIGHT_PAREN, ")", 1),
                Token(TokenType::LEFT_BRACE, "{", 1),
                Token(TokenType::NAME, "x", 2),
                Token(TokenType::EQUAL, "=", 2),
                Token(TokenType::NAME, "x", 2),
                Token(TokenType::PLUS, "+", 2),
                Token(TokenType::INTEGER, "1", 2),
                Token(TokenType::RIGHT_BRACE, "}", 3),
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

TEST_CASE("Test getTokenType") {
    // Positive test cases
    REQUIRE(Token::getTokenType("MyVariable") == TokenType::NAME);
    REQUIRE(Token::getTokenType("123") == TokenType::INTEGER);
    REQUIRE(Token::getTokenType("123") == TokenType::INTEGER);
    REQUIRE(Token::getTokenType("=") == TokenType::EQUAL);
    REQUIRE(Token::getTokenType("+") == TokenType::PLUS);
    REQUIRE(Token::getTokenType("-") == TokenType::MINUS);
    REQUIRE(Token::getTokenType("*") == TokenType::TIMES);
    REQUIRE(Token::getTokenType("/") == TokenType::SLASH);
    REQUIRE(Token::getTokenType("%") == TokenType::PERCENT);
    REQUIRE(Token::getTokenType("&&") == TokenType::AND);
    REQUIRE(Token::getTokenType("||") == TokenType::OR);
    REQUIRE(Token::getTokenType("!") == TokenType::NOT);
    REQUIRE(Token::getTokenType("<") == TokenType::LESS_THAN);
    REQUIRE(Token::getTokenType("<=") == TokenType::LESS_THAN_EQUAL);
    REQUIRE(Token::getTokenType(">") == TokenType::GREATER_THAN);
    REQUIRE(Token::getTokenType(">=") == TokenType::GREATER_THAN_EQUAL);
    REQUIRE(Token::getTokenType("==") == TokenType::DOUBLE_EQUAL);
    REQUIRE(Token::getTokenType("(") == TokenType::LEFT_PAREN);
    REQUIRE(Token::getTokenType(")") == TokenType::RIGHT_PAREN);
    REQUIRE(Token::getTokenType("{") == TokenType::LEFT_BRACE);
    REQUIRE(Token::getTokenType("}") == TokenType::RIGHT_BRACE);
    REQUIRE(Token::getTokenType(";") == TokenType::SEMICOLON);
    REQUIRE(Token::getTokenType("procedure") == TokenType::KEYWORD_PROCEDURE);
    REQUIRE(Token::getTokenType("call") == TokenType::KEYWORD_CALL);
    REQUIRE(Token::getTokenType("print") == TokenType::KEYWORD_PRINT);
    REQUIRE(Token::getTokenType("read") == TokenType::KEYWORD_READ);
    REQUIRE(Token::getTokenType("while") == TokenType::KEYWORD_WHILE);
    REQUIRE(Token::getTokenType("if") == TokenType::KEYWORD_IF);
    REQUIRE(Token::getTokenType("then") == TokenType::KEYWORD_THEN);
    REQUIRE(Token::getTokenType("else") == TokenType::KEYWORD_ELSE);

    // Negative test cases
    REQUIRE(Token::getTokenType("procedures") != TokenType::KEYWORD_PROCEDURE); // keyword spelling is different
    REQUIRE(Token::getTokenType("Procedures") != TokenType::KEYWORD_PROCEDURE); // case-sensitivity

    // Alphanumeric strings
    REQUIRE(Token::getTokenType("procedures123") == TokenType::NAME);



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


