#pragma once

#include <stdexcept>
#include <string>

class QuerySemanticError : public std::runtime_error {
 public:
  explicit QuerySemanticError(const std::string &message)
	  : std::runtime_error(message) {}
};
