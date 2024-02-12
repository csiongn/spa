#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

#include <SP/Token.h>

class Tokenizer {
    public:
        // Constructor
        Tokenizer(const std::string& input);

        void tokenizeProgram();

        std::vector<std::string> tokenizeWord(const std::string& word);

        bool isSpecialToken(char c);

        // Public method to access the tokens
        const std::vector<Token>& getTokens();

    private:
        const std::string& input_;
        std::vector<Token> tokens_;
};

#endif  // TOKENIZER_H