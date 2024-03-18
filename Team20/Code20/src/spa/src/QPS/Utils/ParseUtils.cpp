#include <string>
#include <tuple>

#include "ParseUtils.h"

std::tuple<std::string, bool, bool> QueryEvaluator::ParseUtils::parsePattern(const std::string &pattern) {
  bool hasWildcardLeft = false;
  bool hasWildcardRight = false;
  std::string extractedPattern = pattern;

  // Check for wildcard at the beginning of the pattern
  if (!extractedPattern.empty() && extractedPattern.front() == '_') {
	hasWildcardLeft = true;
	extractedPattern.erase(extractedPattern.begin()); // Remove the wildcard
  }

  // Check for wildcard at the end of the pattern
  if (!extractedPattern.empty() && extractedPattern.back() == '_') {
	hasWildcardRight = true;
	extractedPattern.pop_back(); // Remove the wildcard
  }

  // Trim quotes if present
  if (!extractedPattern.empty() && extractedPattern.front() == '"' && extractedPattern.back() == '"') {
	extractedPattern = extractedPattern.substr(1, extractedPattern.length() - 2);
  }

  return std::make_tuple(extractedPattern, hasWildcardLeft, hasWildcardRight);
}
