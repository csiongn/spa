#include <functional>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "ParseUtils.h"
#include "SP/Token.h"
#include "QPS/QuerySyntaxError.h"

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
	for (SP::TokenType type : types) {
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

std::shared_ptr<ExprNode> QueryEvaluator::ParseUtils::getExprNode(const std::string &pattern) {
  std::vector<Token> tokens = tokenize(pattern);
  std::shared_ptr<ExprNode> node = parse(tokens);
  return node;
}

SimpleProgram::DesignEntity QueryEvaluator::ParseUtils::getSynonymType(std::shared_ptr<QueryToken>& token) {
    std::string tokenValue = token->getValue();
    if (tokenValue == "procedure") {
        return SimpleProgram::DesignEntity::PROCEDURE;
    } else if (tokenValue == "stmt") {
        return SimpleProgram::DesignEntity::STMT;
    } else if (tokenValue == "read") {
        return SimpleProgram::DesignEntity::READ;
    } else if (tokenValue == "print") {
        return SimpleProgram::DesignEntity::PRINT;
    } else if (tokenValue == "assign") {
        return SimpleProgram::DesignEntity::ASSIGN;
    } else if (tokenValue == "call") {
        return SimpleProgram::DesignEntity::CALL;
    } else if (tokenValue == "while") {
        return SimpleProgram::DesignEntity::WHILE;
    } else if (tokenValue == "if") {
        return SimpleProgram::DesignEntity::IF;
    } else if (tokenValue == "variable") {
        return SimpleProgram::DesignEntity::VARIABLE;
    } else if (tokenValue == "constant") {
        return SimpleProgram::DesignEntity::CONSTANT;
    } else {
        return SimpleProgram::DesignEntity{};
    }
}

PQL::Synonym QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity entityType, std::shared_ptr<QueryToken>& token) {
    return {entityType, token->getValue()};
}

PQL::Synonym QueryEvaluator::ParseUtils::createSynonym(SimpleProgram::DesignEntity entityType, std::string synonymIdentity) {
    return {entityType, synonymIdentity};
}

std::vector<std::string> QueryEvaluator::ParseUtils::splitTuple(std::shared_ptr<QueryToken>& tupleToken) {
    std::string tokenValue = tupleToken->getValue();
    const std::regex pattern("<|>");
    auto removedBracketsStr = std::regex_replace(tokenValue, pattern, "");

    std::vector<std::string> res;
    std::istringstream iss(removedBracketsStr);
    std::string token;
    while (std::getline(iss, token, ',')) {
        res.push_back(token);
    }
    return res;
}

std::vector<std::shared_ptr<QueryToken>> QueryEvaluator::ParseUtils::splitTokens(std::vector<std::shared_ptr<QueryToken>>& tokens, int start, int end) {
    return {tokens.begin() + start, tokens.begin() + end};
}

SimpleProgram::DesignEntity QueryEvaluator::ParseUtils::getEntityType(std::shared_ptr<QueryToken>& token, std::vector<PQL::Synonym>& declarations) {
    std::string tokenValue = token->getValue();
    for (auto& declaration : declarations) {
        if (declaration.identity == tokenValue) {
            return declaration.entityType;
        }
    }
}

std::vector<std::shared_ptr<QueryToken>> QueryEvaluator::ParseUtils::removeBracketsAndCommas(std::vector<std::shared_ptr<QueryToken>>& tokens) {
    std::vector<std::shared_ptr<QueryToken>> res;
    std::copy_if(tokens.begin(), tokens.end(), std::back_inserter(res), [](std::shared_ptr<QueryToken> token) {
        return token->getValue() != "," && token->getValue() != "(" && token->getValue() != ")";
    });
    return res;
}

bool QueryEvaluator::ParseUtils::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool QueryEvaluator::ParseUtils::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool QueryEvaluator::ParseUtils::isName(const std::string &str) {
    if (str.empty()) {
        return false; // Empty string is not a valid name
    }

    // Check if the first character is a letter
    if (!isLetter(str[0])) {
        return false;
    }

    // Check if the remaining characters are letters or digits
    for (size_t i = 1; i < str.size(); ++i) {
        if (!isLetter(str[i]) && !isDigit(str[i])) {
            return false;
        }
    }

    return true;
}
