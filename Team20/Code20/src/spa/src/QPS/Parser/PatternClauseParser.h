#pragma once

#include <memory>
#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"
#include "ClauseValidator.h"

class PatternClauseParser {
 protected:
  std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>> &relationshipClauseTokens;
  std::shared_ptr<ClauseValidator> &validator;
  std::vector<PQL::Synonym> &declarations;

 public:
  explicit PatternClauseParser(std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>> &relationshipClauseTokens,
							   std::shared_ptr<ClauseValidator> &validator,
							   std::vector<PQL::Synonym> &declarations);
  std::vector<std::shared_ptr<QueryToken>> getPatternClause();
  SimpleProgram::DesignAbstraction getPatternClauseType(std::shared_ptr<QueryToken> &patternClauseToken);
  bool isNotClause(std::vector<std::shared_ptr<QueryToken>> &patternClauseTokens);
  PQL::Clause parse(std::vector<std::shared_ptr<QueryToken>> &patternClauseTokens);
};