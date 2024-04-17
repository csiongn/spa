#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "SP/AST.h"
#include "SP/Token.h"
#include "QPS/QueryToken.h"
#include "Models/SimpleProgram.h"
#include "Models/PQL.h"

namespace QueryEvaluator {
class EvaluatorUtils {
 public:
  // Custom hash function to hash a set of string
  struct vectorHash {
	size_t operator()(const std::vector<std::string> &vector) const {
	  size_t seed = 0;
	  for (const auto &element : vector) {
		seed ^= std::hash<std::string>()(element) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	  }
	  return seed;
	}
  };

  // Custom hash function to hash a set of string
  struct SetHash {
	size_t operator()(const std::unordered_set<std::string> &set) const {
	  size_t seed = 0;
	  for (const auto &element : set) {
		seed ^= std::hash<std::string>()(element) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	  }
	  return seed;
	}
  };
};
};
