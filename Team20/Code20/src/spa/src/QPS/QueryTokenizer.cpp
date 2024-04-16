#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <memory>
#include <unordered_map>

#include "QueryTokenizer.h"
#include "QueryToken.h"
#include "QuerySyntaxError.h"
#include "../Models/Constants.h"

// tokenize using each character in string
std::vector<std::shared_ptr<QueryToken>> QueryTokenizer::tokenize(const std::string &query) {
  iss = std::make_shared<std::istringstream>(query);
  auto nextChar = peekChar();
  std::string currentStr;
  // Condition remains the same as long as nextChar is not EOF
  while (nextChar != EOF) {
	// while nextChar is still a character, continue to get NAME type
	if (!currentStr.empty() && !isValidIDENT(currentStr)) {
	  // check if invalid string, throw error
	  throw QuerySyntaxError("Syntax Error: Invalid IDENT " + currentStr);
	}
	// Process string within apostrophe
	if (nextChar == Constants::SpecialCharacters::QUOTE) {
	  processApostrophe(); // until apostrophe is closed
	  removeChar();
	} else if (nextChar == Constants::SpecialCharacters::FULL_STOP) {
	  processAttribute(currentStr);
	  currentStr.clear();
	} else if (nextChar == Constants::SpecialCharacters::LEFT_ANGLE_BRACKET) {
	  if (!currentStr.empty()) {
		processString(currentStr);
		currentStr.clear();
	  }
	  processTuple();
	  removeChar();
	} else if (isCharWhitespace(nextChar)) {
	  if (!currentStr.empty()) {
		if (isAttributeString()) {
		  processAttribute(currentStr);
		  currentStr.clear();
		  nextChar = peekChar();
		  continue;
		} else {
		  processString(currentStr);
		  currentStr.clear();
		}
	  }
	  removeChar();
	} else if (isCharStar(nextChar)) {
	  processStar(currentStr);
	  currentStr.clear();
	  removeChar();
	} else if (nextChar == Constants::SpecialCharacters::UNDERSCORE) {
	  processWildcard();
	  removeChar();
	} else if (isCharSeparator(nextChar)) {
	  // Process Separator character
	  processSeparator(currentStr, nextChar);
	  currentStr.clear();
	  removeChar();
	} else if (isCharNumeric(nextChar) || isCharAlpha(nextChar)) {
	  currentStr += nextChar;
	  removeChar();
	} else {
	  throw QuerySyntaxError("Syntax Error for the Invalid token: " + std::string(1, nextChar));
	}
	nextChar = peekChar();
  }
  // Reach EOF
  // if current char is EOF, add remaining string if any
  if (!currentStr.empty()) {
	processString(currentStr);
	currentStr.clear();
  }
  return queryTokens;
}

bool QueryTokenizer::isAlpha(const std::string &str) const {
  return std::all_of(str.begin(), str.end(), [](char c) { return std::isalpha(static_cast<unsigned char>(c)); });
}

bool QueryTokenizer::isNum(const std::string &str) const {
  return std::all_of(str.begin(), str.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
}

bool QueryTokenizer::isAlphanumeric(const std::string &str) const {
  bool hasAlpha = std::any_of(str.begin(), str.end(), ::isalpha);
  bool hasDigit = std::any_of(str.begin(), str.end(), ::isdigit);
  return hasAlpha && hasDigit;
}

bool QueryTokenizer::isNotAlphanumeric(const std::string &str) const {
  bool hasNonAlphaNonDigit = std::any_of(str.begin(), str.end(), [](char c) {
	return !std::isalpha(static_cast<unsigned char>(c)) && !std::isdigit(static_cast<unsigned char>(c));
  });
  return hasNonAlphaNonDigit;
}

bool QueryTokenizer::isCharNumeric(char &c) const {
  return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

bool QueryTokenizer::isCharAlpha(char &c) const {
  return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

bool QueryTokenizer::isCharSeparator(char &c) const {
  return Constants::SpecialCharacters::SEPARATOR_CHARACTERS.find(c)
	  != Constants::SpecialCharacters::SEPARATOR_CHARACTERS.end();
}

bool QueryTokenizer::isCharFactor(char &c) const {
  return Constants::SpecialCharacters::FACTOR_CHARACTERS.find(c)
	  != Constants::SpecialCharacters::FACTOR_CHARACTERS.end();
}

bool QueryTokenizer::isCharStar(char &c) const {
  return c == Constants::SpecialCharacters::STAR;
}

bool QueryTokenizer::isCharWhitespace(char &c) const {
  return Constants::SpecialCharacters::WHITESPACE_CHARACTERS.find(c)
	  != Constants::SpecialCharacters::WHITESPACE_CHARACTERS.end();
}

bool QueryTokenizer::containsFactor(const std::string &str) const {
  // check if str contain special character like \n, ' ', '_', '*', '/', '%', '-', '+', '(' , ')', ','
  return std::any_of(str.begin(), str.end(), [this](char c) {
	return isCharFactor(c);
  });
}

// Cannot start with number
bool QueryTokenizer::isValidIDENT(std::string &str) const {
  if (isAlphanumeric(str) && isCharNumeric(str[0])) {
	return false;
  }
  // IDENT cannot contain special characters, return that only contains digits or letters
  if (isNotAlphanumeric(str)) {
	return false;
  }
  return true;
}

char QueryTokenizer::isStreamEmpty() const {
  return iss->peek() == EOF;
}

char QueryTokenizer::peekChar() const {
  if (isStreamEmpty()) {
	return EOF;
  }
  return static_cast<char>(iss->peek());
}

void QueryTokenizer::removeChar() const {
  if (isStreamEmpty()) {
	return;
  }
  iss->get();
}

void QueryTokenizer::addToken(const std::shared_ptr<QueryToken> &queryToken) {
  queryTokens.push_back(queryToken);
}

// Called when apostrophe is open, and will continue to read until apostrophe is closed
void QueryTokenizer::processApostrophe() {
  // Remove opening apostrophe
  removeChar();
  char nextChar = peekChar();
  std::string currentStr;
  while (nextChar != '"') {
	if (isCharWhitespace(nextChar)) {
	  removeChar();
	  nextChar = peekChar();

	  if (!currentStr.empty() && !containsFactor(currentStr)) {
		// after space is not factor special characters, throw error
		if (isCharNumeric(nextChar) || isCharAlpha(nextChar)) {
		  throw QuerySyntaxError(
			  "Syntax Error: Invalid space within apostrophe expression: " + std::string(1, nextChar));
		}
	  }
	} else {
	  currentStr += nextChar;
	  removeChar();
	  nextChar = peekChar();
	}
  }

  if (!currentStr.empty()) {
	// check if want to do constant_string or expression tokens
	// Need to check for invalid expression special character
	if (isNotAlphanumeric(currentStr)) {
	  // Means this is an expression most likely
	  std::string currExprStr = "";
	  for (char c : currentStr) {
		if (isCharNumeric(c) || isCharAlpha(c) || isCharFactor(c)) {
		  currExprStr += c;
		} else {
		  // Unsupported char within apostrophe for expression
		  throw QuerySyntaxError(
			  "Syntax Error: Invalid character within apostrophe expression: " + std::string(1, c));
		}
	  }
	  addToken(std::make_shared<QueryToken>(QPS::TokenType::EXPRESSION, currExprStr));
	  currentStr.clear();
	} else {
	  // Not an expression but a single token, cant check if valid IDENT or NAME or INTEGER here(valid var_name, const_value)
	  if (isValidIDENT(currentStr) || isNum(currentStr) || isAlpha(currentStr)) {
		addToken(std::make_shared<QueryToken>(QPS::TokenType::CONSTANT_STRING, currentStr));
		currentStr.clear();
	  } else {
		throw QuerySyntaxError("Syntax Error: Invalid IDENT within apostrophe: " + currentStr);
	  }
	}
  }
}

void QueryTokenizer::processStar(std::string currentStr) {
  // if current string is non empty, follows NAME rule, add to tokens, and apostrophe must not be open
  if (!currentStr.empty()) {
	currentStr += Constants::SpecialCharacters::STAR;
	addToken(std::make_shared<QueryToken>(QPS::TokenType::NAME, currentStr));
	currentStr.clear();
  } else if (currentStr.empty()) {
	throw QuerySyntaxError("Syntax Error for the following star token after empty string: *");
  }
}

void QueryTokenizer::processWildcard() {
  addToken(std::make_shared<QueryToken>(QPS::TokenType::WILDCARD, "_"));
}

void QueryTokenizer::processSeparator(std::string currentStr, char separatorChar) {

  if (!currentStr.empty() && isNum(currentStr)) {
	addToken(std::make_shared<QueryToken>(QPS::TokenType::INTEGER, currentStr)); // string or name
	// isValidIDENT
  } else if (!currentStr.empty() && isValidIDENT(currentStr)) {
	// Cannot start with number, must adhere to IDENT
	addToken(std::make_shared<QueryToken>(QPS::TokenType::NAME, currentStr)); // string or name
  }
  if (separatorChar != Constants::SpecialCharacters::LEFT_ANGLE_BRACKET) {
	addToken(std::make_shared<QueryToken>(QPS::TokenType::SPECIAL_CHARACTER, std::string(1, separatorChar)));
  }
}

void QueryTokenizer::processAttributeToken(std::string currentAttributeStr, std::string attributeType) {
  std::unordered_map<std::string, QPS::TokenType> attributeValueMap = {
	  {"stmt#", QPS::TokenType::ATTRIBUTE_VALUE},
	  {"procName", QPS::TokenType::ATTRIBUTE_NAME},
	  {"varName", QPS::TokenType::ATTRIBUTE_NAME},
	  {"value", QPS::TokenType::ATTRIBUTE_CONSTANT}
  };
  std::string attributeTokenValue = currentAttributeStr + attributeType;
  if (!isValidAttributeField(attributeType)) {
	throw QuerySyntaxError("Syntax Error: Invalid attribute type: " + attributeType);
  }
  addToken(std::make_shared<QueryToken>(attributeValueMap[attributeType], attributeTokenValue));
}

void QueryTokenizer::processAttribute(std::string currentStr) {
  if (currentStr.empty()) {
	throw QuerySyntaxError("Syntax Error: Invalid whitespace before attribute");
  }
  std::string currAttributeString = currentStr;
  std::string currAttributeValue = "";
  // Process until whitespace
  auto nextChar = peekChar();
  if (nextChar == Constants::SpecialCharacters::FULL_STOP) {
	currAttributeString += nextChar;
	removeChar();
  }

  nextChar = peekChar();
  // Remove whitespace after `.` and before attribute
  while (isCharWhitespace(nextChar) && nextChar != EOF) {
	removeChar();
	nextChar = peekChar();
  }

  while (nextChar != Constants::SpecialCharacters::SPACE && nextChar != EOF && nextChar != Constants::SpecialCharacters::EQUAL) {
	currAttributeValue += nextChar;
	removeChar();
	nextChar = peekChar();
  }
  processAttributeToken(currAttributeString, currAttributeValue);
  return;
}

void QueryTokenizer::processTuple() {
  // Process from < to >
  std::string currentStr;
  // start from <
  auto nextChar = peekChar();
  currentStr += nextChar;
  removeChar();
  nextChar = peekChar();
  std::string currArg;
  bool isFirstArg = true;
  while (nextChar != Constants::SpecialCharacters::RIGHT_ANGLE_BRACKET && nextChar != EOF) {
	// Not whitespace
	if (Constants::SpecialCharacters::WHITESPACE_CHARACTERS.find(nextChar)
		== Constants::SpecialCharacters::WHITESPACE_CHARACTERS.end()) {
	  if (nextChar == Constants::SpecialCharacters::COMMA) {
		// immediately add comma to currStr dont wait till in else block
		if (currArg.empty()) {
		  currentStr += nextChar;
		  removeChar();
		  nextChar = peekChar();
		  continue;
		} else if (currArg.find(Constants::SpecialCharacters::FULL_STOP) != std::string::npos) {
		  // exclude fullstop
		  auto attributeValue = currArg.substr(currArg.find(Constants::SpecialCharacters::FULL_STOP) + 1);
		  if (!isValidAttributeField(attributeValue)) {
			throw QuerySyntaxError("Syntax Error: Invalid attribute type: " + attributeValue);
		  } else {
			// Valid attribute add to currentStr first thn add comma
			if (!isFirstArg) {
			  if (currentStr.back() != Constants::SpecialCharacters::COMMA
				  && currArg.front() != Constants::SpecialCharacters::COMMA) {
				throw QuerySyntaxError(
					"Syntax Error: [COMMA BRANCH ADD ATTR]Invalid tuple No Comma before arg");
			  }
			}
			currentStr += currArg;
			isFirstArg = false;
			currArg.clear();
		  }
		} else {
		  if (!isFirstArg) {
			if (currentStr.back() != Constants::SpecialCharacters::COMMA
				&& currArg.front() != Constants::SpecialCharacters::COMMA) {
			  throw QuerySyntaxError("Syntax Error: [COMMA BRANCH Not ATTR]Invalid tuple No Comma before arg");
			}
		  }
		  currentStr += currArg;
		  isFirstArg = false;
		  currArg.clear();
		}
	  }
	  currArg += nextChar;
	} else {
	  if (!currArg.empty()) {
		checkTupleAttribute(currArg);
		if (!isFirstArg) {
		  if (currentStr.back() != Constants::SpecialCharacters::COMMA
			  && currArg != std::string(1, Constants::SpecialCharacters::COMMA)) {
			throw QuerySyntaxError("Syntax Error: [WS branch]Invalid tuple No Comma before arg" + currentStr + currArg);
		  }
		}
		currentStr += currArg;
		isFirstArg = false;
		currArg.clear();
	  }
	}
	removeChar();
	nextChar = peekChar();
  }
  if (nextChar != Constants::SpecialCharacters::RIGHT_ANGLE_BRACKET) {
	throw QuerySyntaxError("Syntax Error: Invalid tuple Missing `>`: " + currentStr);
  }
  if (!currArg.empty()) {
	if (!isFirstArg) {
	  if (currentStr.back() != Constants::SpecialCharacters::COMMA
		  && currArg.front() != Constants::SpecialCharacters::COMMA) {
		throw QuerySyntaxError(
			"Syntax Error: [Exit While]Invalid tuple No Comma before arg" + currentStr + " " + currArg);
	  }
	}
	currentStr += currArg;
  }

  checkTupleAttribute(currArg);
  // Add right angle bracket
  currentStr += nextChar;
  checkTupleComma(currentStr);

  addToken(std::make_shared<QueryToken>(QPS::TokenType::TUPLE, currentStr));
  return;
}

void QueryTokenizer::processString(std::string currentStr) {

  if (isNum(currentStr) && !currentStr.empty()) {
	addToken(std::make_shared<QueryToken>(QPS::TokenType::INTEGER, currentStr));
  } else if (isValidIDENT(currentStr)) {
	addToken(std::make_shared<QueryToken>(QPS::TokenType::NAME, currentStr));
  } else {
	throw QuerySyntaxError("Syntax Error: Invalid IDENT " + currentStr);
  }
  return;
}

bool QueryTokenizer::isAttributeString() {
  // restore to last position with whitespace
  std::streampos restorePos;
  int posToRestore = 0;
  auto nextChar = peekChar();
  while (isCharWhitespace(nextChar) && nextChar != EOF) {
	restorePos = iss->tellg();
	posToRestore += 1;
	removeChar();
	nextChar = peekChar();
  }
  if (nextChar == Constants::SpecialCharacters::FULL_STOP) {
	// All whitespace has been removed
	return true;
  }
  iss->seekg(restorePos);
  return false;
}

bool QueryTokenizer::isValidAttributeField(std::string attributeField) {
  std::unordered_map<std::string, QPS::TokenType> attributeValueMap = {
	  {"stmt#", QPS::TokenType::ATTRIBUTE_VALUE},
	  {"procName", QPS::TokenType::ATTRIBUTE_NAME},
	  {"varName", QPS::TokenType::ATTRIBUTE_NAME},
	  {"value", QPS::TokenType::ATTRIBUTE_CONSTANT}
  };
  if (attributeValueMap.find(attributeField) == attributeValueMap.end()) {
	return false;
  }
  return true;
}

void QueryTokenizer::checkTupleComma(std::string tupleString) {
  // Check if currentStr contains comma
  // Strip away left and right angle brackets
  auto tupleContent = tupleString.substr(1, tupleString.size() - 2);
  // Split tuple content into string by comma
  std::stringstream ss(tupleContent);
  std::string token;
  std::vector<std::string> tupleTokens;
  while (std::getline(ss, token, Constants::SpecialCharacters::COMMA)) {
	tupleTokens.push_back(token);
  }

  if (!tupleContent.empty() && tupleContent.back() == Constants::SpecialCharacters::COMMA) {
	tupleTokens.push_back("");
  }

  for (const auto &tupleToken : tupleTokens) {
	if (tupleToken.empty()) {
	  throw QuerySyntaxError("Syntax Error: Invalid tuple " + tupleString);
	}
  }
  return;
}

// Throws error if is a attribute and have invalid attribute field
void QueryTokenizer::checkTupleAttribute(std::string currArg) {
  if (currArg.find(Constants::SpecialCharacters::FULL_STOP) != std::string::npos) {
	// exclude fullstop
	auto attributeValue = currArg.substr(currArg.find(Constants::SpecialCharacters::FULL_STOP) + 1);
	if (!isValidAttributeField(attributeValue)) {
	  throw QuerySyntaxError("Syntax Error: Invalid attribute type: " + attributeValue);
	}
  }
  return;
}