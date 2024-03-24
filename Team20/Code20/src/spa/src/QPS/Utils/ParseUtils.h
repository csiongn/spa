#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "SP/AST.h"
#include "SP/Token.h"

namespace QueryEvaluator {
class ParseUtils {
 public:
  static std::tuple<std::string, bool, bool> parsePattern(const std::string &pattern);

  static std::shared_ptr<ExprNode> getExprNode(const std::string &pattern);

  static std::vector<Token> tokenize(const std::string &pattern);

  static std::shared_ptr<ExprNode> parse(std::vector<Token> &tokens);
};
};
