#pragma once

#include <stdexcept>
#include <string>

class QuerySyntaxError : public std::runtime_error {
 public:
  explicit QuerySyntaxError(const std::string &message)
	  : std::runtime_error(message) {}
};
