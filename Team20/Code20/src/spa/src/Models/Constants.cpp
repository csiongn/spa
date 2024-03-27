#include "Constants.h"

namespace Constants {
const char SpecialCharacters::COMMA = ',';
const char SpecialCharacters::OPEN_BRACKET = '(';
const char SpecialCharacters::CLOSE_BRACKET = ')';
const char SpecialCharacters::SEMICOLON = ';';
const char SpecialCharacters::SPACE = ' ';
const char SpecialCharacters::UNDERSCORE = '_';
const char SpecialCharacters::STAR = '*';
const char SpecialCharacters::EQUAL = '=';
const char SpecialCharacters::MULTIPLY = '*';
const char SpecialCharacters::PLUS = '+';
const char SpecialCharacters::MINUS = '-';
const char SpecialCharacters::DIVIDE = '/';
const char SpecialCharacters::MOD = '%';
const char SpecialCharacters::FULL_STOP = '.';
const char SpecialCharacters::QUOTE = '"';
const char SpecialCharacters::LEFT_ANGLE_BRACKET = '<';
const char SpecialCharacters::RIGHT_ANGLE_BRACKET = '>';
const char SpecialCharacters::HASH_SYMBOL = '#';

// Whitepace
const char SpecialCharacters::TAB = '\t';
const char SpecialCharacters::NEWLINE = '\n';

// set will remove duplicates of STAR and MULTIPLY
const std::unordered_set<char> SpecialCharacters::SPECIAL_CHARACTERS = {
	COMMA, OPEN_BRACKET, CLOSE_BRACKET, SEMICOLON, UNDERSCORE, STAR,
	EQUAL, MULTIPLY, PLUS, MINUS, DIVIDE, MOD, FULL_STOP, QUOTE, LEFT_ANGLE_BRACKET,
	RIGHT_ANGLE_BRACKET, HASH_SYMBOL
};

const std::unordered_set<char> SpecialCharacters::SEPARATOR_CHARACTERS = {
	COMMA, OPEN_BRACKET, CLOSE_BRACKET, SEMICOLON, EQUAL
};
// Expression can have these characters
const std::unordered_set<char> SpecialCharacters::FACTOR_CHARACTERS = {
	EQUAL, MULTIPLY, PLUS, MINUS, DIVIDE, MOD, OPEN_BRACKET, CLOSE_BRACKET
};

const std::unordered_set<char> SpecialCharacters::NON_FACTOR_CHARACTERS = {
	COMMA, OPEN_BRACKET, CLOSE_BRACKET, SEMICOLON, SPACE, UNDERSCORE, FULL_STOP,
	QUOTE, LEFT_ANGLE_BRACKET, RIGHT_ANGLE_BRACKET, HASH_SYMBOL
};

const std::unordered_set<char> SpecialCharacters::WHITESPACE_CHARACTERS = {
	SPACE, TAB, NEWLINE
};

const std::unordered_set<char> SpecialCharacters::TUPLE_CHARACTERS = {
	LEFT_ANGLE_BRACKET, RIGHT_ANGLE_BRACKET
};

const std::unordered_set<char> SpecialCharacters::ATTRIBUTE_CHARACTERS = {
	FULL_STOP, HASH_SYMBOL
};
}