#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "SP/AST.h"
#include "SP/Token.h"
#include "QPS/QueryToken.h"
#include "Models/SimpleProgram.h"
#include "Models/PQL.h"

namespace QueryEvaluator {
class ParseUtils {
 public:
  static std::tuple<std::string, bool, bool> parsePattern(const std::string &pattern);

  static std::shared_ptr<ExprNode> getExprNode(const std::string &pattern);

  static std::vector<Token> tokenize(const std::string &pattern);

  static std::shared_ptr<ExprNode> parse(std::vector<Token> &tokens);

  static SimpleProgram::DesignEntity getSynonymType(std::shared_ptr<QueryToken>& token);

  static PQL::Synonym createSynonym(SimpleProgram::DesignEntity& entityType, std::shared_ptr<QueryToken>& token);

  static PQL::Synonym createSynonym(SimpleProgram::DesignEntity &entityType, std::string synonymIdentity);

  static std::vector<std::string> splitTuple(std::shared_ptr<QueryToken>& tupleToken);

  static std::vector<std::shared_ptr<QueryToken>> splitTokens(std::vector<std::shared_ptr<QueryToken>>& tokens, int start, int end);

  static SimpleProgram::DesignEntity getEntityType(std::shared_ptr<QueryToken>& token, std::vector<PQL::Synonym>& declarations);

  static std::vector<std::shared_ptr<QueryToken>> removeBracketsAndCommas(std::vector<std::shared_ptr<QueryToken>>& tokens);

  static bool isLetter(char c);

  static bool isDigit(char c);

  static bool isName(const std::string &str);
  };
};
