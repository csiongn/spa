#pragma once

#include <stdexcept>
#include <string>
#include "QueryError.h"

class QuerySyntaxError : public QueryError {
 public:
  explicit QuerySyntaxError(const std::string &message) : QueryError(message) {}
};
