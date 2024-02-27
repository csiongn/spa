#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "QueryTokenizer.h"
#include "QueryToken.h"
#include "QuerySyntaxError.h"

// tokenize using each character in string
std::vector<std::shared_ptr<QueryToken>> QueryTokenizer::tokenize(const std::string &query) {
    std::vector<std::shared_ptr<QueryToken>> queryTokens;
    auto iss = std::istringstream(query);
    // read from stream directly instead of line by line splitted by whitespace
    // peek and get returns int, so we need to cast to char
    char nextChar = static_cast<char>(iss.peek());
    bool isApostropheOpen = false;
    std::string currentStr;
    bool hasSpaceinApostrophe = false;
    while (nextChar != EOF) {
        // while nextChar is still a character, continue to get NAME type
        if (!currentStr.empty()) {
            // check if invalid string, throw error
            if (isAlpha(currentStr) && isCharNumeric(currentStr[0])) {
                throw QuerySyntaxError("Syntax Error for the following token: " + currentStr);
            }
        }
        // isWhitespace, currenStr is alnum, and nextChar is char invalid token
        // if nextChar is whitespace, skip
        if (nextChar == ' ' || nextChar == '\n' || nextChar == '\t') {
            // if current string is non empty, follows NAME rule, add to tokens, and apostrophe must not be open
            // Add currentStr as NAME token and clear
            if (!isApostropheOpen) {

                if (!currentStr.empty()) {
                    queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::NAME, currentStr));
                    currentStr.clear();
                }
            } else {
                hasSpaceinApostrophe = true;
            }
            // continue on, currentStr may continue to be added
            iss.get(); // remove from stream
            nextChar = static_cast<char>(iss.peek());
            continue;
        } else if (nextChar == '"' && isApostropheOpen) {
            isApostropheOpen = !isApostropheOpen;

            if (!currentStr.empty()) {
                // check if want to do constant_string or expression tokens
                if (containsFactorSpecialChar(currentStr)) {
                    // use currentStr and iterate through each character to store
                    for (char c : currentStr) {
                        if (isCharFactorSpecialChar(c)) {
                            queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::SPECIAL_CHARACTER, std::string(1, c)));
                        } else {
                            queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::NAME, std::string(1, c)));
                        }
                    }
                    currentStr.clear();
                    hasSpaceinApostrophe = false;
                } else if (hasSpaceinApostrophe) {
                    throw QuerySyntaxError("Syntax Error for the following token: " + currentStr);
                } else {
                    queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::CONSTANT_STRING, currentStr));
                    currentStr.clear();
                }
            }
            // move on
            iss.get(); // remove from stream
            nextChar = static_cast<char>(iss.peek());

        } else if (nextChar == '"' && !isApostropheOpen) {
            isApostropheOpen = !isApostropheOpen;
            iss.get(); // remove from stream
            nextChar = static_cast<char>(iss.peek());
        } else if (isApostropheOpen) {
            // check if character in string is invalid, throw error
            currentStr += nextChar;
            iss.get();
            nextChar = static_cast<char>(iss.peek());
            continue;
        } else if (isCharStar(nextChar)) {
            // Follows*
            if (!currentStr.empty()) {
                currentStr += nextChar;
                queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::NAME, currentStr));
                currentStr.clear();
            }
            iss.get();
            nextChar = static_cast<char>(iss.peek());
            continue;
        } else if (nextChar == '_') {
            // shouldnt need to care about string before and after, will be handled
            queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::WILDCARD, "_"));
            iss.get();
            nextChar = static_cast<char>(iss.peek());
            continue;
        } else if (isCharSpecialChar(nextChar)) {
            // if current string is non empty, follow NAME rule, add to tokens, and apostrophe must not be open
            if (!currentStr.empty() && isNum(currentStr)) {
                queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::INTEGER, currentStr)); // string or name
                currentStr.clear();
            } else if (!currentStr.empty() && !isCharNumeric(currentStr[0]) && (isAlphanumeric(currentStr) || isAlpha(currentStr))) {
                queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::NAME, currentStr)); // string or name
                currentStr.clear();
            }
            queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::SPECIAL_CHARACTER, std::string(1, nextChar)));
            iss.get();
            nextChar = static_cast<char>(iss.peek());
            continue;
        } else if (isCharNumeric(nextChar) || isCharAlpha(nextChar)) {
            currentStr += nextChar;
            iss.get();
            nextChar = static_cast<char>(iss.peek());
        } else {
            throw QuerySyntaxError("Syntax Error for the following token: " + nextChar);
        }
    }

    // if current char is EOF, add remaining string if any
    if (!currentStr.empty()) {
        queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::NAME, currentStr));
    }

    return queryTokens;
}


bool QueryTokenizer::startsWithLetter(const std::string& str) const {
    if (str.empty()) {
        return false;
    }
    return std::isalpha(str[0]) != 0;
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

bool QueryTokenizer::isCharNumeric(char& c) const {
    return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

bool QueryTokenizer::isCharAlpha(char& c) const {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

bool QueryTokenizer::isCharSpecialChar(char& c) const {
    return c == '_' || c == '/' || c == '%' || c == '-' || c == '+' || c == '(' || c == ')' || c == '*' || c == ',' || c == ';';
}

bool QueryTokenizer::isCharFactorSpecialChar(char& c) const {
    return c == '*' || c == '/' || c == '%' || c == '-' || c == '+';
}

bool QueryTokenizer::isCharStar(char& c) const {
    return c == '*';
}

bool QueryTokenizer::isCharWhitespace(char& c) const {
    return c == ' ';
}

bool QueryTokenizer::containsSpecialChar(const std::string &str) const {
    // check if str contain special character like \n, ' ', '_', '*', '/', '%', '-', '+', '(' , ')', ','
    return std::any_of(str.begin(), str.end(), [](char c) {
        return c == '_' || c == '*' || c == '/' || c == '%' || c == '-' || c == '+' || c == '(' || c == ')' || c == ',' || c == ';';
    });
}

bool QueryTokenizer::containsFactorSpecialChar(const std::string &str) const {
    // check if str contain special character like \n, ' ', '_', '*', '/', '%', '-', '+', '(' , ')', ','
    return std::any_of(str.begin(), str.end(), [](char c) {
        return c == '*' || c == '/' || c == '%' || c == '-' || c == '+';
    });
}
