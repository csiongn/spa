#pragma once

#include <cctype>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

// Fixed TokenTypes, to change this enum, do change tokenMapping
namespace SP {
enum class TokenType {
  NAME,
  INTEGER,
  EQUAL,
  PLUS,
  MINUS,
  TIMES,
  SLASH,
  PERCENT,
  EOFILE, // Equal used for assignment
  AND,
  OR,
  NOT,
  LESS_THAN,
  LESS_THAN_EQUAL,
  GREATER_THAN,
  GREATER_THAN_EQUAL,
  DOUBLE_EQUAL,
  NOT_EQUAL, // Double equal used as comparator
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  SEMICOLON,
  KEYWORD_PROCEDURE,
  KEYWORD_CALL,
  KEYWORD_PRINT,
  KEYWORD_READ,
  KEYWORD_WHILE,
  KEYWORD_IF,
  KEYWORD_THEN,
  KEYWORD_ELSE
};
}

const static std::vector<SP::TokenType> keywords = {
	SP::TokenType::KEYWORD_PROCEDURE,
	SP::TokenType::KEYWORD_IF,
	SP::TokenType::KEYWORD_THEN,
	SP::TokenType::KEYWORD_ELSE,
	SP::TokenType::KEYWORD_READ,
	SP::TokenType::KEYWORD_CALL,
	SP::TokenType::KEYWORD_PRINT,
	SP::TokenType::KEYWORD_WHILE
};

/// Maps token types to specific strings.
/// excludes INTEGER and NAME as they do not have 1:1 mapping with a string
const std::unordered_map<std::string, SP::TokenType> tokenMapping = {
	{"=", SP::TokenType::EQUAL},
	{"+", SP::TokenType::PLUS},
	{"-", SP::TokenType::MINUS},
	{"*", SP::TokenType::TIMES},
	{"/", SP::TokenType::SLASH},
	{"%", SP::TokenType::PERCENT},
	{"&&", SP::TokenType::AND},
	{"||", SP::TokenType::OR},
	{"!", SP::TokenType::NOT},
	{"<", SP::TokenType::LESS_THAN},
	{"<=", SP::TokenType::LESS_THAN_EQUAL},
	{">", SP::TokenType::GREATER_THAN},
	{">=", SP::TokenType::GREATER_THAN_EQUAL},
	{"==", SP::TokenType::DOUBLE_EQUAL},
	{"!=", SP::TokenType::NOT_EQUAL},
	{"(", SP::TokenType::LEFT_PAREN},
	{")", SP::TokenType::RIGHT_PAREN},
	{"{", SP::TokenType::LEFT_BRACE},
	{"}", SP::TokenType::RIGHT_BRACE},
	{";", SP::TokenType::SEMICOLON},
	{"procedure", SP::TokenType::KEYWORD_PROCEDURE},
	{"call", SP::TokenType::KEYWORD_CALL},
	{"print", SP::TokenType::KEYWORD_PRINT},
	{"read", SP::TokenType::KEYWORD_READ},
	{"while", SP::TokenType::KEYWORD_WHILE},
	{"if", SP::TokenType::KEYWORD_IF},
	{"then", SP::TokenType::KEYWORD_THEN},
	{"else", SP::TokenType::KEYWORD_ELSE}
};

struct Token {
  // Attributes
  SP::TokenType type;
  std::string value;
  std::int16_t line_num;

  // Constructor
  Token(SP::TokenType type, std::string text) : type(type), value(std::move(text)) {}
  Token(SP::TokenType type, std::string text, std::int16_t line_num)
	  : type(type), value(std::move(text)), line_num(line_num) {}

  // Default constructor for empty declaration
  Token() : type(SP::TokenType::NAME), value(""), line_num() {}

  // Equality operator
  bool operator==(const Token &other) const {
	return type == other.type && value == other.value;
  }

  // Inequality operator
  bool operator!=(const Token &other) const {
	return !(*this == other);
  }

  /// Checks the SP::TokenType of a given token_string
  static SP::TokenType TokenType(const std::string &token_string) {
	auto it = tokenMapping.find(token_string);
	if (it != tokenMapping.end()) {
	  return it->second;
	}

	// If first char is alpha, then it is a legit variable name in SIMPLE
	if (std::isalpha(token_string[0])) {
	  return SP::TokenType::NAME;
	}

	// Throw exception for non-legit non-alphanumeric characters such as ===
	if (!std::isalnum(token_string[0])) {
	  throw std::runtime_error("Syntax error: invalid token " + token_string);
	}

	// Throw exception for illegal name with number at the start e.g. 123name
	if (std::isdigit(token_string[0])) { // Start of string is digit
	  for (char c : token_string) {
		if (!std::isdigit(c)) { // but somewhere in the string, a non-digit exists
		  throw std::runtime_error("Syntax error: invalid token " + token_string);
		}
	  }
	}

	// If the token is not found in the mapping and it is not a digit, assume it is a NAME token
	return SP::TokenType::INTEGER;
  }

  /// Checks if string is in token mapping.
  static bool isStringInTokenMapping(const std::string &key) {
	auto result = tokenMapping.find(key);
	return result != tokenMapping.end();
  }

  /// Checks if string is supposed to be a unique symbol I.e.
  /// - It is len(1)
  /// - It does not have any combination with any other symbol e.g. =, > is not counted as it can form >=
  static bool isUniqueSymbol(const std::string &key) {
	auto result = tokenMapping.find(key);
	if (result != tokenMapping.end()) {
	  switch (result->second) {
		case SP::TokenType::LEFT_BRACE:
		case SP::TokenType::LEFT_PAREN:
		case SP::TokenType::RIGHT_BRACE:
		case SP::TokenType::RIGHT_PAREN:
		case SP::TokenType::SEMICOLON:
		  return true;
		default:
		  return false;
	  }
	}
  }

  /// Returns string representation of token with value and line_num
  std::string toString() const {
	return "Token(" + value + "\", " + std::to_string(line_num) + ")";
  }
};
