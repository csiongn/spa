#pragma once

#include <unordered_set>

namespace Constants {
    struct SpecialCharacters {
        static const char COMMA;
        static const char OPEN_BRACKET;
        static const char CLOSE_BRACKET;
        static const char SEMICOLON;
        static const char SPACE;
        static const char UNDERSCORE;
        static const char FULL_STOP;
        static const char QUOTE;
        static const char LEFT_ANGLE_BRACKET;
        static const char RIGHT_ANGLE_BRACKET;
        static const char HASH_SYMBOL;
        // doubles as multiply
        static const char STAR;
        static const char EQUAL;
        static const char MULTIPLY;
        static const char PLUS;
        static const char MINUS;
        static const char DIVIDE;
        static const char MOD;

        // whitespace
        static const char TAB;
        static const char NEWLINE;

        // Different sets of special characters
        static const std::unordered_set<char> SPECIAL_CHARACTERS;
        static const std::unordered_set<char> SEPARATOR_CHARACTERS;
        static const std::unordered_set<char> FACTOR_CHARACTERS;
        static const std::unordered_set<char> NON_FACTOR_CHARACTERS;
        static const std::unordered_set<char> WHITESPACE_CHARACTERS;

    };
}
