#pragma once

#include <stdexcept>
#include <string>

class QuerySemanticError : public QueryError {
 public:
  explicit QuerySemanticError(const std::string &message) : QueryError(message) {}
};
