#pragma once
#include <sstream>
#include <vector>
#include "QueryToken.h"

class QueryTokenizer {

private:
    void processApostrophe();

    void processStar(std::string currentStr);

    void processWildcard();

    void processSeparator(std::string currentStr, char separatorChar);

protected:
    // Move stream to an attribute in enclosing class so that helper functions can access
    std::shared_ptr<std::istream> iss;
    std::vector<std::shared_ptr<QueryToken>> queryTokens;

public:

    explicit QueryTokenizer() = default;

    std::vector<std::shared_ptr<QueryToken> > tokenize(const std::string &query);

    bool isAlpha(const std::string& str) const;

    bool isNum(const std::string& str) const;

    bool isAlphanumeric(const std::string& str) const;

    bool isCharNumeric(char& c) const;

    bool isCharAlpha(char& c) const;

    bool containsFactor(const std::string &str) const;

    bool isCharFactor(char &c) const;

    bool isCharStar(char &c) const;

    bool isCharWhitespace(char &c) const;

    bool isValidIDENT(std::string &str) const;

    bool isNotAlphanumeric(const std::string &str) const;

    bool isCharSeparator(char &c) const;

    char isStreamEmpty() const;
    // Function to peek but not pop character
    char peekChar() const;
    // Function to pop character
    void removeChar() const;

    void addToken(const std::shared_ptr<QueryToken> & queryToken);

};