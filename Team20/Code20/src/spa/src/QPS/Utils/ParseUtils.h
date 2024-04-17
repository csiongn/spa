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

  static SimpleProgram::DesignEntity getSynonymType(std::shared_ptr<QueryToken> &token);

  static PQL::Synonym createSynonym(SimpleProgram::DesignEntity entityType, std::shared_ptr<QueryToken> &token);

  static PQL::Synonym createSynonym(SimpleProgram::DesignEntity entityType, std::string synonymIdentity);

  static PQL::Synonym createSynonym(SimpleProgram::DesignEntity entityType,
									std::string synonymIdentity,
									SimpleProgram::AttributeRef attrRef);

  static std::vector<std::string> splitTuple(std::shared_ptr<QueryToken> &tupleToken);

  static std::vector<std::shared_ptr<QueryToken>> splitTokens(std::vector<std::shared_ptr<QueryToken>> &tokens,
															  int start,
															  int end);

  static SimpleProgram::DesignEntity getEntityType(std::shared_ptr<QueryToken> &token,
												   std::vector<PQL::Synonym> &declarations);

  static SimpleProgram::DesignEntity getEntityType(std::string synonymIdentity,
												   std::vector<PQL::Synonym> &declarations);

  static std::vector<std::shared_ptr<QueryToken>> removeBracketsAndCommas(std::vector<std::shared_ptr<QueryToken>> &tokens);

  static bool isLetter(char c);

  static bool isDigit(char c);

  static bool isName(const std::string &str);

  static std::vector<std::shared_ptr<QueryToken>> getClause(std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>> &relationshipClauseTokens);

  static std::vector<std::string> splitStrByFirstDelim(std::string str, std::string delim);

  static std::vector<std::string> splitAttrToken(std::shared_ptr<QueryToken> attrToken);

  static SimpleProgram::AttributeRef getAttrRef(std::string attrRefStr);

  static PQL::Synonym createSynonymFromAttrToken(std::shared_ptr<QueryToken> attrToken,
												 std::vector<PQL::Synonym> declarations);

  static PQL::Synonym createAttrSynonym(std::shared_ptr<QueryToken> operandToken,
										std::vector<PQL::Synonym> declarations);

  static PQL::Synonym createAttrSynonym(std::string attrRefStr, std::vector<PQL::Synonym> declarations);
};
};
