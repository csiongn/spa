#pragma once
#include <sstream>
#include "QueryToken.h"
class QueryTokenizer {
protected:
    std::istream *inputStream;

public:
    explicit QueryTokenizer() = default;
    // Cannot send in query here, need to return the tokenizer
    std::vector<std::shared_ptr<QueryToken> > tokenize(std::string &query);

    std::vector<QueryToken> tokenizeIndividualStr(const std::string &individualStr);

    bool startsWithLetter(const std::string& str) const;

    bool isAlpha(const std::string& str) const;

    bool isNum(const std::string& str) const;

    bool isAlphanumeric(const std::string& str) const;

    bool isCharNumeric(char& c) const;

    bool isCharAlpha(char& c) const;

    bool containsSpecialChar(const std::string &str) const;

    bool isCharSpecialChar(char &c) const;

    bool isCharStar(char &c) const;
};