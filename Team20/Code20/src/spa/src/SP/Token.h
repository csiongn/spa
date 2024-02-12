#pragma once
#include <cctype>
#include <map>
#include <string>
#include <unordered_map>

// Fixed TokenTypes, to change this enum, do change tokenMapping
enum class TokenType {
    NAME, INTEGER, EQUAL, PLUS, MINUS, TIMES, SLASH, PERCENT, // Equal used for assignment
    AND, OR, NOT, 
    LESS_THAN, LESS_THAN_EQUAL, GREATER_THAN, GREATER_THAN_EQUAL, DOUBLE_EQUAL, // Double equal used as comparator
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, SEMICOLON,
    KEYWORD_PROCEDURE, KEYWORD_CALL, KEYWORD_PRINT, KEYWORD_READ, KEYWORD_WHILE, KEYWORD_IF, KEYWORD_THEN, KEYWORD_ELSE
};

/// <summary>
/// Maps token types to specific strings.
/// excludes INTEGER and NAME as they do not have 1:1 mapping with a string
/// </summary>
const std::unordered_map<std::string, TokenType> tokenMapping = {
    {"=", TokenType::EQUAL},
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::TIMES},
    {"/", TokenType::SLASH},
    {"%", TokenType::PERCENT},
    {"&&", TokenType::AND},
    {"||", TokenType::OR},
    {"!", TokenType::NOT},
    {"<", TokenType::LESS_THAN},
    {"<=", TokenType::LESS_THAN_EQUAL},
    {">", TokenType::GREATER_THAN},
    {">=", TokenType::GREATER_THAN_EQUAL},
    {"==", TokenType::DOUBLE_EQUAL},
    {"(", TokenType::LEFT_PAREN},
    {")", TokenType::RIGHT_PAREN},
    {"{", TokenType::LEFT_BRACE},
    {"}", TokenType::RIGHT_BRACE},
    {";", TokenType::SEMICOLON},
    {"procedure", TokenType::KEYWORD_PROCEDURE},
    {"call", TokenType::KEYWORD_CALL},
    {"print", TokenType::KEYWORD_PRINT},
    {"read", TokenType::KEYWORD_READ},
    {"while", TokenType::KEYWORD_WHILE},
    {"if", TokenType::KEYWORD_IF},
    {"then", TokenType::KEYWORD_THEN},
    {"else", TokenType::KEYWORD_ELSE}
};

struct Token {
    TokenType type;
    std::string value;
    std::int16_t line_num;

    Token(TokenType type, std::string text, std::int16_t line_num)
        : type(type), value(std::move(text)), line_num(line_num) {}

    // Default constructor for empty declaration
    Token() : type(TokenType::NAME), value(""), line_num() {}

    // Equality operator
    bool operator==(const Token& other) const {
        return type == other.type && value == other.value;
    }

    // Inequality operator
    bool operator!=(const Token& other) const {
        return !(*this == other);
    }

    /// <summary>
    /// Checks the TokenType of a given token_string
    /// </summary>
    /// <param name="token_string"></param>
    /// <returns>TokenType type: type of the token</returns>
    static TokenType getTokenType(const std::string& token_string) {
        auto it = tokenMapping.find(token_string);
        if (it != tokenMapping.end()) {
            return it->second;
        }

        // If first char is alpha, then it is a legit variable name in SIMPLE
        if (std::isalpha(token_string[0])) {
            return TokenType::NAME;
        }

        // TODO: Implement exception for non-legit non-alphanumeric characters such as ===
        // TODO: Implement exception for strings with number at the start e.g. 123name

        // If the token is not found in the mapping and it is not a digit, assume it is a NAME token
        return TokenType::INTEGER;
    }

    /// <summary>
    /// Checks if string is in token mapping.
    /// </summary>
    /// <param name="key">String input</param>
    /// <returns>True if string is in mapping</returns>
    static bool isStringInTokenMapping(const std::string& key) {
        auto result = tokenMapping.find(key);
        if (result == tokenMapping.end()) {
            return false;
        }
        else {
            return true;
        }
    }

};