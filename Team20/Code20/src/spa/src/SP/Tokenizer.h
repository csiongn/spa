#pragma once

#include <string>
#include <vector>

#include "SP/Token.h"

class Tokenizer {
 public:
  // Constructor
  explicit Tokenizer(const std::string &input);

  void tokenizeProgram();

  std::vector<std::string> tokenizeWord(const std::string &word);
  std::vector<Token> tokenizeLineStream(std::istringstream &lineStream, std::int16_t line_num);

  bool isSpecialToken(char c);

  // Public method to access the tokens
  const std::vector<Token> &getTokens();

 private:
  const std::string &input_;
  std::vector<Token> tokens_;
};
