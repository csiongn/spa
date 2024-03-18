#pragma once

#include <string>
#include <tuple>

namespace QueryEvaluator {
class ParseUtils {

 public:
  static std::tuple<std::string, bool, bool> parsePattern(const std::string &pattern);
};
};