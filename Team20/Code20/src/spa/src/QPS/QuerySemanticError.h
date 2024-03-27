#pragma once

#include <stdexcept>
#include <string>
#include "QueryError.h"

class QuerySemanticError : public QueryError {
 public:
  explicit QuerySemanticError(const std::string &message) : QueryError(message) {}
};
