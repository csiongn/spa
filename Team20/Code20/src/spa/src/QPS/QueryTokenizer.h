#pragma once
#include <sstream>
#include "QueryToken.h"
class QueryTokenizer {
public:
    std::istream *inputStream;

    explicit QueryTokenizer() = default;

    std::vector<std::shared_ptr<QueryToken> > tokenize(const std::string &query);

    bool startsWithLetter(const std::string& str) const;

    bool isAlpha(const std::string& str) const;

    bool isNum(const std::string& str) const;

    bool isAlphanumeric(const std::string& str) const;

    bool isCharNumeric(char& c) const;

    bool isCharAlpha(char& c) const;

    bool containsSpecialChar(const std::string &str) const;

    bool containsFactorSpecialChar(const std::string &str) const;

    bool isCharSpecialChar(char &c) const;

    bool isCharFactorSpecialChar(char &c) const;

    bool isCharStar(char &c) const;

    bool isCharWhitespace(char &c) const;
};