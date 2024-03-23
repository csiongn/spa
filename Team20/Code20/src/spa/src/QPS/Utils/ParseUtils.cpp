#include <functional>
#include <memory>
#include <regex>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "ParseUtils.h"
#include "SP/Token.h"

std::tuple<std::string, bool, bool> QueryEvaluator::ParseUtils::parsePattern(const std::string &pattern) {
    bool hasWildcardLeft = false;
    bool hasWildcardRight = false;
    std::string extractedPattern = pattern;

    // Check for wildcard at the beginning of the pattern
    if (!extractedPattern.empty() && extractedPattern.front() == '_') {
        hasWildcardLeft = true;
        extractedPattern.erase(extractedPattern.begin()); // Remove the wildcard
    }

    // Check for wildcard at the end of the pattern
    if (!extractedPattern.empty() && extractedPattern.back() == '_') {
        hasWildcardRight = true;
        extractedPattern.pop_back(); // Remove the wildcard
    }

    // Trim quotes if present
    if (!extractedPattern.empty() && extractedPattern.front() == '"' && extractedPattern.back() == '"') {
        extractedPattern = extractedPattern.substr(1, extractedPattern.length() - 2);
    }

    return std::make_tuple(extractedPattern, hasWildcardLeft, hasWildcardRight);
}

std::vector<Token> QueryEvaluator::ParseUtils::tokenize(const std::string &pattern) {
    // ai-gen start(copilot, 1, e)
    // prompt: Use copilot
    std::regex tokenRegex(R"(\s*([a-zA-Z]+\d*|\d+|[+\-*/%()])\s*)");
    // ai-gen end
    std::vector<Token> tokens;
    std::sregex_iterator iter(pattern.begin(), pattern.end(), tokenRegex);
    std::sregex_iterator end;

    while (iter != end) {
        std::smatch match = *iter;
        std::string tokenStr = match.str(1);
        SP::TokenType tokenType = Token::TokenType(tokenStr);
        tokens.emplace_back(tokenType, tokenStr);
        ++iter;
    }

    tokens.emplace_back(SP::TokenType::EOFILE, "");

    return tokens;
}


std::shared_ptr<ExprNode> QueryEvaluator::ParseUtils::parse(std::vector<Token> &tokens) {
    int current = 0;

    // Forward declaration of parsing functions
    std::function<std::shared_ptr<ExprNode>()> expression;
    std::function<std::shared_ptr<ExprNode>()> term;
    std::function<std::shared_ptr<ExprNode>()> factor;
    std::function<Token(SP::TokenType)> consume;
    std::function<bool(SP::TokenType)> check;
    std::function<Token()> advance;
    std::function<bool(const std::vector<SP::TokenType> &)> match;
    std::function<bool()> isAtEnd;
    std::function<bool()> isKeyword;

    expression = [&]() {
        // Capture enclosing scope variables by reference
        std::shared_ptr<ExprNode> expr = term();

        while (match({SP::TokenType::PLUS, SP::TokenType::MINUS})) {
            std::string op = tokens[current - 1].value;
            std::shared_ptr<ExprNode> right = term();
            expr = std::make_shared<BinaryExprNode>(std::move(expr), op, std::move(right));
        }

        return expr;
    };

    term = [&]() {
        std::shared_ptr<ExprNode> expr = factor();

        while (match({SP::TokenType::TIMES, SP::TokenType::SLASH, SP::TokenType::PERCENT})) {
            std::string op = tokens[current - 1].value;
            std::shared_ptr<ExprNode> right = factor();
            expr = std::make_shared<BinaryExprNode>(std::move(expr), op, std::move(right));
        }

        return expr;
    };

    factor = [&]() {
        std::shared_ptr<ExprNode> expr;
        if (match({SP::TokenType::LEFT_PAREN})) {
            expr = expression();
            consume(SP::TokenType::RIGHT_PAREN);
        } else if (match({SP::TokenType::INTEGER, SP::TokenType::NAME})) {
            std::string value = tokens[current - 1].value;
            if (tokens[current - 1].type == SP::TokenType::INTEGER) {
                expr = std::make_shared<LiteralNode>(value);
            } else {
                expr = std::make_shared<VariableNode>(value);
            }
        }
        return expr;
    };

    consume = [&](SP::TokenType type) {
        if (check(type)) return advance();
    };

    check = [&](SP::TokenType type) {
        if (isAtEnd()) return false;
        return tokens[current].type == type ||
               (type == SP::TokenType::NAME && isKeyword());
        // Allow token type to be keyword when we expect a name token
    };

    match = [&](const std::vector<SP::TokenType> &types) {
        for (SP::TokenType type: types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    };

    advance = [&]() {
        if (!isAtEnd()) current++;
        return tokens[current - 1];
    };

    isAtEnd = [&]() {
        return tokens[current].type == SP::TokenType::EOFILE;
    };

    isKeyword = [&]() {
        SP::TokenType type = tokens[current].type;
        auto it = std::find(keywords.begin(), keywords.end(), type);
        return it != keywords.end();
    };

    return expression();
}
