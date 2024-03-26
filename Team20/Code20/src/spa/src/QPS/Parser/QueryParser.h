#pragma once

#include <vector>
#include "QPS/QueryToken.h"
#include "Models/PQL.h"

class QueryParser {
 protected:
  std::vector<std::shared_ptr<QueryToken>> tokens;

  std::vector<PQL::Synonym> declarations;

 public:
  explicit QueryParser(std::vector<std::shared_ptr<QueryToken>> tokens);

  PQL::Query parse();

 private:
  std::vector<int> parseSemicolons();
};