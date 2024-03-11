#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "QueryTokenizer.h"
#include "QueryToken.h"
#include "QuerySyntaxError.h"
#include "../Models/Constants.h"
using namespace Constants;

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
        if (nextChar == '"') {
            processApostrophe(); // until apostrophe is closed
            nextChar = peekChar();
        } else if (isCharWhitespace(nextChar)) {
            // if current string is non empty, follows NAME rule, add to tokens, and apostrophe must not be open
            // Add currentStr as NAME token and clear
            if (!currentStr.empty()) {
                addToken(std::make_shared<QueryToken>(QPS::TokenType::NAME, currentStr));
                currentStr.clear();
            }
            removeChar(); // remove from stream
            nextChar = peekChar();
            continue;
        } else if (isCharStar(nextChar)) {
            processStar(currentStr);
            currentStr.clear();
            removeChar();
            nextChar = peekChar();
            continue;
        } else if (nextChar == '_') {
            // shouldnt need to care about string before and after, will be handled
            processWildcard();
            nextChar = peekChar();
            continue;
        } else if (isCharSeparator(nextChar)) {
            // Process Separator character
            processSeparator(currentStr, nextChar);
            currentStr.clear();
            nextChar = peekChar();
            continue;
        } else if (isCharNumeric(nextChar) || isCharAlpha(nextChar)) {
            currentStr += nextChar;
            removeChar();
            nextChar = peekChar();
        } else {
            throw QuerySyntaxError("Syntax Error for the Invalid token: " + std::string(1, nextChar));
        }
    }

    // if current char is EOF, add remaining string if any
    if (!currentStr.empty()) {
        addToken(std::make_shared<QueryToken>(QPS::TokenType::NAME, currentStr));
    }

    // clean up iss
    return queryTokens;
}


bool QueryTokenizer::isAlpha(const std::string& str) const {
    return std::all_of(str.begin(), str.end(), [](char c){ return std::isalpha(static_cast<unsigned char>(c)); });
}

bool QueryTokenizer::isNum(const std::string& str) const {
    return std::all_of(str.begin(), str.end(), [](char c){ return std::isdigit(static_cast<unsigned char>(c)); });
}

bool QueryTokenizer::isAlphanumeric(const std::string& str) const {
    bool hasAlpha = std::any_of(str.begin(), str.end(), ::isalpha);
    bool hasDigit = std::any_of(str.begin(), str.end(), ::isdigit);
    return hasAlpha && hasDigit;
}

bool QueryTokenizer::isNotAlphanumeric(const std::string& str) const {
    bool hasNonAlphaNonDigit = std::any_of(str.begin(), str.end(), [](char c) {
        return !std::isalpha(static_cast<unsigned char>(c)) && !std::isdigit(static_cast<unsigned char>(c));
    });
    return hasNonAlphaNonDigit;
}

bool QueryTokenizer::isCharNumeric(char& c) const {
    return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

bool QueryTokenizer::isCharAlpha(char& c) const {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

bool QueryTokenizer::isCharSeparator(char& c) const {
    return SpecialCharacters::SEPARATOR_CHARACTERS.find(c) != SpecialCharacters::SEPARATOR_CHARACTERS.end();
}

bool QueryTokenizer::isCharFactor(char& c) const {
    return SpecialCharacters::FACTOR_CHARACTERS.find(c) != SpecialCharacters::FACTOR_CHARACTERS.end();
}

bool QueryTokenizer::isCharStar(char& c) const {
    return c == SpecialCharacters::STAR;
}

bool QueryTokenizer::isCharWhitespace(char& c) const {
    return SpecialCharacters::WHITESPACE_CHARACTERS.find(c) != SpecialCharacters::FACTOR_CHARACTERS.end();
}

bool QueryTokenizer::containsFactor(const std::string &str) const {
    // check if str contain special character like \n, ' ', '_', '*', '/', '%', '-', '+', '(' , ')', ','
    return std::any_of(str.begin(), str.end(), [this](char c) {
        return isCharFactor(c);
    });
}

// Cannot start with number
bool QueryTokenizer::isValidIDENT(std::string &str) const {
    if(isAlphanumeric(str) && isCharNumeric(str[0])){
        return false;
    }
    // IDENT cannot contain special characters, return that only contains digits or letters
    if(isNotAlphanumeric(str)){
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

            if(!currentStr.empty() && !containsFactor(currentStr)) {
                // after space is not factor special characters, throw error
                if (isCharNumeric(nextChar) || isCharAlpha(nextChar)) {
                    throw QuerySyntaxError("Syntax Error: Invalid space within apostrophe expression: " + std::string(1, nextChar));
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
            for (char c: currentStr) {
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
    // Remove closing apostrophe
    removeChar();
}


void QueryTokenizer::processStar(std::string currentStr) {
    // if current string is non empty, follows NAME rule, add to tokens, and apostrophe must not be open
    if (!currentStr.empty()) {
        currentStr += SpecialCharacters::STAR;
        addToken(std::make_shared<QueryToken>(QPS::TokenType::NAME, currentStr));
        currentStr.clear();
    } else if (currentStr.empty()) {
        throw QuerySyntaxError("Syntax Error for the following star token after empty string: *");
    }
}

void QueryTokenizer::processWildcard() {
    addToken(std::make_shared<QueryToken>(QPS::TokenType::WILDCARD, "_"));
    removeChar();
}

void QueryTokenizer::processSeparator(std::string currentStr, char separatorChar) {

    if (!currentStr.empty() && isNum(currentStr)) {
        addToken(std::make_shared<QueryToken>(QPS::TokenType::INTEGER, currentStr)); // string or name
        currentStr.clear();
        // isValidIDENT
    } else if (!currentStr.empty() && isValidIDENT(currentStr)) {
        // Cannot start with number, must adhere to IDENT
        addToken(std::make_shared<QueryToken>(QPS::TokenType::NAME, currentStr)); // string or name
        currentStr.clear();
    }
    addToken(std::make_shared<QueryToken>(QPS::TokenType::SPECIAL_CHARACTER, std::string(1, separatorChar)));
    removeChar();
}
