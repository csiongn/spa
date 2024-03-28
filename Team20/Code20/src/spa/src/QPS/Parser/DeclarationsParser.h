#pragma once

#include <memory>
#include <vector>
#include "Models/PQL.h"
#include "QPS/QueryToken.h"
#include "DeclarationsValidator.h"

class DeclarationsParser {
 protected:
  std::vector<std::shared_ptr<QueryToken>> declarationTokens;
  std::vector<int> semicolonPos;
  std::shared_ptr<DeclarationsValidator> &validator;

 public:
  explicit DeclarationsParser(std::vector<std::shared_ptr<QueryToken>> declarationTokens,
							  std::vector<int> semicolonPos,
							  std::shared_ptr<DeclarationsValidator> &validator);

  std::vector<PQL::Synonym> parseDeclarations();
};