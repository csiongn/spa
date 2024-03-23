#include <vector>

#include "catch.hpp"
#include "QPS/QueryToken.h"
#include "QPS/Utils/ParseUtils.h"
#include "SP/Token.h"

std::vector<Token> createTokens(const std::vector<std::pair<SP::TokenType, std::string>>& tokenData) {
    std::vector<Token> tokens;
    for (const auto& [type, value] : tokenData) {
        tokens.emplace_back(type, value);
    }
    tokens.emplace_back(SP::TokenType::EOFILE, "");
    return tokens;
}

TEST_CASE("Tokenization") {
    SECTION("Single delimiter") {
        std::vector<Token> expected = {
            Token(SP::TokenType::NAME, "a"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::INTEGER, "2"),
            Token(SP::TokenType::EOFILE, "")
        };
        std::vector<Token> actual = QueryEvaluator::ParseUtils::tokenize("a+2");
        REQUIRE(expected == actual);
    }

    SECTION("Multiple delimiters") {
        std::vector<Token> expected = {
            Token(SP::TokenType::NAME, "a"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::INTEGER, "2"),
            Token(SP::TokenType::PERCENT, "%"),
            Token(SP::TokenType::NAME, "abc123"),
            Token(SP::TokenType::EOFILE, "")
        };
        std::vector<Token> actual = QueryEvaluator::ParseUtils::tokenize("a+2%abc123");
        REQUIRE(expected == actual);
    }

    SECTION("Multiple delimiters – whitespaces") {
        std::vector<Token> expected = {
            Token(SP::TokenType::NAME, "a"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::INTEGER, "2"),
            Token(SP::TokenType::PERCENT, "%"),
            Token(SP::TokenType::NAME, "abc123"),
            Token(SP::TokenType::EOFILE, "")
        };
        std::vector<Token> actual = QueryEvaluator::ParseUtils::tokenize("  a  +    \n     2       %      abc123 ");
        REQUIRE(expected == actual);
    }
}

TEST_CASE("Tokenize simple expressions", "[tokenize]") {
    SECTION("Single variable name") {
        auto tokens = QueryEvaluator::ParseUtils::tokenize("x");
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].type == SP::TokenType::NAME);
        REQUIRE(tokens[0].value == "x");
        REQUIRE(tokens[1].type == SP::TokenType::EOFILE);
    }

    SECTION("Integer literal") {
        auto tokens = QueryEvaluator::ParseUtils::tokenize("123");
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].type == SP::TokenType::INTEGER);
        REQUIRE(tokens[0].value == "123");
        REQUIRE(tokens[1].type == SP::TokenType::EOFILE);
    }

    SECTION("Arithmetic expression") {
        auto tokens = QueryEvaluator::ParseUtils::tokenize("x+2");
        REQUIRE(tokens.size() == 4);
        REQUIRE(tokens[0].type == SP::TokenType::NAME);
        REQUIRE(tokens[1].type == SP::TokenType::PLUS);
        REQUIRE(tokens[2].type == SP::TokenType::INTEGER);
        REQUIRE(tokens[3].type == SP::TokenType::EOFILE);
    }

    SECTION("Expression with keywords as names") {
        auto tokens = QueryEvaluator::ParseUtils::tokenize("if+while%procedure444");
        REQUIRE(tokens.size() == 6);
        REQUIRE(tokens[0].type == SP::TokenType::KEYWORD_IF);
        REQUIRE(tokens[1].type == SP::TokenType::PLUS);
        REQUIRE(tokens[2].type == SP::TokenType::KEYWORD_WHILE);
        REQUIRE(tokens[3].type == SP::TokenType::PERCENT);
        REQUIRE(tokens[4].type == SP::TokenType::NAME);
        REQUIRE(tokens[5].type == SP::TokenType::EOFILE);
    }

    SECTION("Complex expression with parentheses") {
        auto tokens = QueryEvaluator::ParseUtils::tokenize("(x+2)*3");
        REQUIRE(tokens.size() == 8);
        REQUIRE(tokens[0].type == SP::TokenType::LEFT_PAREN);
        REQUIRE(tokens[1].type == SP::TokenType::NAME);
        REQUIRE(tokens[2].type == SP::TokenType::PLUS);
        REQUIRE(tokens[3].type == SP::TokenType::INTEGER);
        REQUIRE(tokens[4].type == SP::TokenType::RIGHT_PAREN);
        REQUIRE(tokens[5].type == SP::TokenType::TIMES);
        REQUIRE(tokens[6].type == SP::TokenType::INTEGER);
        REQUIRE(tokens[7].type == SP::TokenType::EOFILE);
    }
}

TEST_CASE("Parsing single token expressions") {
    SECTION("Simple expressions") {
        auto luckyToken = createTokens({{SP::TokenType::INTEGER, "888"}});
        auto result = QueryEvaluator::ParseUtils::parse(luckyToken);

        auto literalNode = std::dynamic_pointer_cast<LiteralNode>(result);
        REQUIRE_FALSE(nullptr == result);
        REQUIRE("888" == literalNode->value);
    }

    SECTION("Single keyword token") {
        auto ifToken = createTokens({{SP::TokenType::KEYWORD_IF, "if"}});
        auto result = QueryEvaluator::ParseUtils::parse(ifToken);

        auto variableNode = std::dynamic_pointer_cast<VariableNode>(result);
        REQUIRE_FALSE(nullptr == result);
        REQUIRE("if" == variableNode->value);
    }
}

TEST_CASE("Parsing multiple token expressions") {
    SECTION("Simple expressions") {
        auto tokens = createTokens({
            {SP::TokenType::INTEGER, "888"},
            {SP::TokenType::PLUS, "+"},
            {SP::TokenType::NAME, "abc"}
        });
        auto result = QueryEvaluator::ParseUtils::parse(tokens);

        auto binaryExprNode = std::dynamic_pointer_cast<BinaryExprNode>(result);
        REQUIRE_FALSE(nullptr == result);
        REQUIRE("+" == binaryExprNode->op);
        REQUIRE("888" == std::dynamic_pointer_cast<LiteralNode>(binaryExprNode->left)->value);
        REQUIRE("abc" == std::dynamic_pointer_cast<VariableNode>(binaryExprNode->right)->value);
    }

    SECTION("Keyword tokens") {
        auto tokens = createTokens({
                    {SP::TokenType::KEYWORD_IF, "if"},
                    {SP::TokenType::PLUS, "+"},
                    {SP::TokenType::NAME, "maybe"},
                    {SP::TokenType::TIMES, "*"},
                    {SP::TokenType::KEYWORD_WHILE, "while"}
                });

        auto result = QueryEvaluator::ParseUtils::parse(tokens);

        auto binaryExprNode = std::dynamic_pointer_cast<BinaryExprNode>(result);
        REQUIRE_FALSE(nullptr == result);
        REQUIRE("+" == binaryExprNode->op);

        REQUIRE("if" == std::dynamic_pointer_cast<VariableNode>(binaryExprNode->left)->value);

        auto right = std::dynamic_pointer_cast<BinaryExprNode>(binaryExprNode->right);
        REQUIRE_FALSE(nullptr == right);
        REQUIRE("*" == right->op);
        REQUIRE("maybe" == std::dynamic_pointer_cast<VariableNode>(right->left)->value);
        REQUIRE("while" == std::dynamic_pointer_cast<VariableNode>(right->right)->value);
    }
}
