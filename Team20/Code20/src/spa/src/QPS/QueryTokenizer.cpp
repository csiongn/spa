#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "QueryTokenizer.h"
#include "QueryToken.h"
#include "QuerySyntaxError.h"

// Start sending query throught tokenizer.tokenize, so that we can reuse tokenizer
std::vector<std::shared_ptr<QueryToken>> QueryTokenizer::tokenize(const std::string &query) {
    std::vector<std::shared_ptr<QueryToken>> queryTokens;
    auto iss = std::istringstream(query);
    std::string line;

    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string individualStr;
        while (lineStream >> individualStr) {

            std::vector<QueryToken> tokens = tokenizeIndividualStr(individualStr);
            // APPEND TOKENS
            for (auto token : tokens) {
                queryTokens.emplace_back(std::make_shared<QueryToken>(token));
            }
        }
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
    return c == '\n' || c == ' ' || c == '_' || c == '/' || c == '%' || c == '-' || c == '+' || c == '(' || c == ')' || c == '*' || c == ',' || c == ';';
}

bool QueryTokenizer::isCharStar(char& c) const{
    return c == '*';
}

// Take in individual string separated from line with whitespace
std::vector<QueryToken> QueryTokenizer::tokenizeIndividualStr(const std::string& individualStr) {
    std::cout << "individualStr: " << individualStr << std::endl;
    // strip first and last " " from query
    std::vector<QueryToken> tokens;
    std::string currentStr;
    bool isApostropheOpen = false;

    // We can get guarantee it is a NAME token eg. Select , s , such , that
    if (startsWithLetter(individualStr) && isAlphanumeric(individualStr) && !containsSpecialChar(individualStr)) {
        // emplace_back is better than push_back for complex object from online sources
        tokens.emplace_back(QueryToken(TokenType::NAME, individualStr));
        currentStr.clear();
    } else if (isAlpha(individualStr) && !containsSpecialChar(individualStr)) {
        tokens.emplace_back(QueryToken(TokenType::NAME, individualStr));
        currentStr.clear();
    }
    // error if issa string which start with number
    else {

        // Follows(1,s) , would be one line
        // Follows(1, "x*y/3")
        for (char c: individualStr) {
            // check is currentStr is alphanum but start with number -> not allowed
            if (currentStr.size() > 1 && isAlphanumeric(currentStr) && isCharNumeric(currentStr[0])) {
                throw QuerySyntaxError("Syntax Error for the following token: " + currentStr);
            }
            // if is alpha we read up to the word aka last alphabet then add to currentStr
            // Can still start with alpha
            if (isCharAlpha(c)) {
                currentStr += c;
            } else if (isCharNumeric(c)) {
                currentStr += c;
            } else if (c == '"') {
                // this can be a "x", "x*y", "main", "var123aa"
                // get string until next "
                if (isApostropheOpen) {
                    tokens.emplace_back(QueryToken(TokenType::CONSTANT_STRING, currentStr));
                    currentStr.clear();
                    isApostropheOpen = false;
                    continue;
                } else {
                    isApostropheOpen = true;
                    continue; // move to next loop to get the string out
                }

            } else if (!isApostropheOpen && isCharSpecialChar(c)) { // \n, ' ', '_', '*', '/', '%', '-', '+' omitted
                // if currentStr is not empty, add to tokens
                // could be in the middle of string makes it invalid syntax
                if (!currentStr.empty() && isNum(currentStr)) {
                    tokens.emplace_back(QueryToken(TokenType::INTEGER, currentStr)); // string or name
                    currentStr.clear();
                } else if (!currentStr.empty() && !isCharNumeric(currentStr[0]) && (isAlphanumeric(currentStr) || isAlpha(currentStr))) {
                    tokens.emplace_back(QueryToken(TokenType::NAME, currentStr)); // string or name
                    currentStr.clear();
                } else if (!currentStr.empty() && isAlphanumeric(currentStr) && isCharNumeric(currentStr[0])) {
                    throw QuerySyntaxError("Syntax Error for the following token: " + currentStr);
                }   else if (!currentStr.empty() && isCharStar(c)) {
                    currentStr += c;
                }   else if (!currentStr.empty() && isCharSpecialChar(c)) {
                    throw QuerySyntaxError("Syntax Error for the following token: " + currentStr);
                }
                tokens.emplace_back(QueryToken(TokenType::SPECIAL_CHARACTER, std::string(1, c)));
            } else if (c == '_') { // wildcard
                tokens.emplace_back(QueryToken(TokenType::WILDCARD, "_"));
            } else if (isCharNumeric(c)) {
                currentStr += c; // form the number token to be added
            } else { // only allow within " "
                // \n, ' ', '_', '*', '/', '%', '-', '+' omitted
                if (isApostropheOpen) {
                    currentStr += c;
                }
                std::cout << "no operations yet: " << c << '\n';
            }
        }
    }
    return tokens;
}

bool QueryTokenizer::containsSpecialChar(const std::string &str) const {
    // check if str contain special character like \n, ' ', '_', '*', '/', '%', '-', '+', '(' , ')', ','
    return std::any_of(str.begin(), str.end(), [](char c) {
        return c == '\n' || c == ' ' || c == '_' || c == '*' || c == '/' || c == '%' || c == '-' || c == '+' || c == '(' || c == ')';
    });
}




