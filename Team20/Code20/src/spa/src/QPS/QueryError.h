#pragma once

#include <stdexcept>
#include <string>

class QueryError : public std::runtime_error {
 public:
  explicit QueryError(const std::string &message) : std::runtime_error(message) {}
};
