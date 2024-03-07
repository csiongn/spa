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
    auto nextChar = static_cast<char>(iss.peek());
    bool isApostropheOpen = false;
    std::string currentStr;

    while (nextChar != EOF) {
        // while nextChar is still a character, continue to get NAME type
        if (!currentStr.empty()) {
            // check if invalid string, throw error
            if (!isValidIDENT(currentStr) && !isApostropheOpen) {
                throw QuerySyntaxError("Syntax Error: Invalid IDENT " + currentStr);
            }

            // TODO: Does not check IDENT string when isApostropheOpen, need to check if currentStr is alpha or alphanumeric, ignore when is numeric
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
                    std::string currIDENT = "";
                    for (char c : currentStr) {
                        if (isCharFactorSpecialChar(c)) {
                            queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::SPECIAL_CHARACTER, std::string(1, c)));
                        } else if (isCharNumeric(c)) {
                            // Add additional check to split token to correct type for parser to use
                            queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::INTEGER, std::string(1, c)));
                        } else {
                            // TODO: NAME inside apostrophe being passed 1 char by 1 char
                            // TODO: need to add method to read the NAME token inside apostrophe
                            queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::NAME, std::string(1, c)));
                        }
                    }


                    currentStr.clear();
                    // TODO: Remove this check, not error to have whitespace within 1 CONSTANT STRING within apostrophe
                } else {
                    // TODO: Do we still want CONSTANT_STRING type for single token within apostrophe
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
            // TODO: Whitespace checking in apostropheOpen to ignore it
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
            } else if (currentStr.empty()) {
                throw QuerySyntaxError("Syntax Error for the following star token after empty string: *");
            }
            iss.get();
            nextChar = static_cast<char>(iss.peek());
            // Check that nextChar after star character is not whitespace when it is not within apostrophe
            if (isCharWhitespace(nextChar)) {
                throw QuerySyntaxError(
                        "Syntax Error: the current string with next char after star char being whitespace");
            }
            continue;
        } else if (nextChar == '_') {
            // shouldnt need to care about string before and after, will be handled
            queryTokens.emplace_back(std::make_shared<QueryToken>(TokenType::WILDCARD, "_"));
            iss.get();
            nextChar = static_cast<char>(iss.peek());
            continue;
        } else if (isCharSpecialCharSeparator(nextChar)) {
            // ! Change to only add to tokens, when next char is separator, and not supposed to be part of string
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
            throw QuerySyntaxError("Syntax Error for the Invalid token: " + std::string(1, nextChar));
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

bool QueryTokenizer::isNotAlphanumric(const std::string& str) const {
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

bool QueryTokenizer::isCharSpecialChar(char& c) const {
    return c == '_' || c == '/' || c == '%' || c == '-' || c == '+' || c == '(' || c == ')' || c == '*' || c == ',' || c == ';';
}

bool QueryTokenizer::isCharSpecialCharIDENT(char& c) const {
    return c == '_' || c == '/' || c == '%' || c == '-' || c == '+' || c == '(' || c == ')' || c == '*' || c == ',' || c == ';' || c == '&';
}

bool QueryTokenizer::isCharSpecialCharSeparator(char& c) const {
    return  c == '(' || c == ')' || c == ',' || c == ';';
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

bool QueryTokenizer::isValidIDENT(std::string &str) const {
    if(isAlphanumeric(str) && isCharNumeric(str[0])){
        return false;
    }
    // IDENT cannot contain special characters, return that only contains digits or letters
    if(isNotAlphanumric(str)){
        return false;
    }
    return true;
}