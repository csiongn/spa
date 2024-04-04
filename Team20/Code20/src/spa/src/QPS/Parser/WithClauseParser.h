#pragma once

#include <memory>
#include <string>
#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"
#include "ClauseValidator.h"

class WithClauseParser {
 protected:
  std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>> &relationshipClauseTokens;
  std::shared_ptr<ClauseValidator> &validator;
  std::vector<PQL::Synonym> &declarations;

 public:
  explicit WithClauseParser(std::shared_ptr<std::vector<std::shared_ptr<QueryToken>>> &relationshipClauseTokens,
							std::shared_ptr<ClauseValidator> &validator,
							std::vector<PQL::Synonym> &declarations);
  std::vector<std::shared_ptr<QueryToken>> getWithClause();
  PQL::Clause parse(std::vector<std::shared_ptr<QueryToken>> &withClauseTokens);

 private:
  SimpleProgram::AttributeRef getAttrRef(std::string attrRefStr);
  PQL::Synonym createSynonymFromAttrToken(std::shared_ptr<QueryToken> attrToken);
};
