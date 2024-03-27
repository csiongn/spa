#pragma once

#include <memory>
#include <vector>
#include <string>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"
#include "ClauseValidator.h"

class ClauseParser {
 protected:
  std::vector<std::shared_ptr<QueryToken>> clauseTokens;

  std::vector<PQL::Synonym> &declarations;

  std::shared_ptr<ClauseValidator> &validator;

 public:
  explicit ClauseParser(std::vector<std::shared_ptr<QueryToken>> &clauseTokens,
						std::vector<PQL::Synonym> &declarations,
						std::shared_ptr<ClauseValidator> &validator);

  PQL::Synonym getDeclarationUsed(std::shared_ptr<QueryToken> synonymToken);

  PQL::Synonym getDeclarationUsed(std::string synonymIdentity);

  std::vector<PQL::Synonym> parseSelectClause();

  std::vector<PQL::Clause> parseRelationshipClause();
};
