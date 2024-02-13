#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <SP/Tokenizer.h>

Tokenizer::Tokenizer(const std::string& input) : input_(input) {}

/// <summary>
/// Tokenizes program and stores the tokens in tokenizer
/// </summary>
/// <param name="program">string representation of input SIMPLE code</param>
void Tokenizer::tokenizeProgram() {
    std::vector<Token> tokens;
    std::istringstream iss(input_);
    std::string line;
    std::int16_t lineNum = 1;

    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string word;

        while (lineStream >> word) {
            std::vector<std::string> wordTokens = tokenizeWord(word);
            for (const auto& token : wordTokens) {
                SP::TokenType t_type = Token::TokenType(token);
                Token t(t_type, token, lineNum);
                tokens_.push_back(t);
            }
        }

        lineNum++;
    }

    // TODO: Remove hardcoded EOF
    Token t(SP::TokenType::EOFILE, "", lineNum);
    tokens_.push_back(t);
}

/*
Checks if string has a delimiter attached at the end or in the middle. 
E.g. x + y = z;
The 'z;' substring should split into 'z' and ';'

Special characters that are together will form a token by itself.
E.g. x===y;
Should give "x", "===", "y"

TODO: Refactor code to be more neat
*/
std::vector<std::string> Tokenizer::tokenizeWord(const std::string& word) {
    std::vector<std::string> tokens;
    std::string currentToken;

    for (size_t i = 0; i < word.length(); ++i) {
        char c = word[i];
        if (isSpecialToken(c)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            currentToken += c;

            // Check for multi-length tokens e.g. ==
            while (i + 1 < word.length() && isSpecialToken(word[i + 1])) {
                currentToken += word[++i];
            }
            tokens.push_back(currentToken);
            currentToken.clear();
        }
        else {
            currentToken += c;
        }
    }

    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    return tokens;
}

/*
* Checks if character is a delimiter(special token) defined in Token::SP::TokenType
* Delimiter is defined as non-alphanumeric characters
*/ 
bool Tokenizer::isSpecialToken(char c) {
    std::string s = std::string(1, c);
    return Token::isStringInTokenMapping(s);
}

/// <summary>
/// Returns tokens stored in tokenizer. Each token has the fields:
/// SP::TokenType type, string value, int16_t line)
/// </summary>
/// <returns>list of tokens stored</returns>
const std::vector<Token>& Tokenizer::getTokens(){
    return tokens_;
}
