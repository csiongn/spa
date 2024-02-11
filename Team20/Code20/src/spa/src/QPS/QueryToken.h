#pragma once
#include <iostream>
#include <unordered_map>


// Can be quotation mark string or variable name
// entities declaration: procedure p, variable v, constant c, stmt s, call c, while w, if i, assign a
// First letter capitalise for Clauses, Select, Follows, Parent
// Select s such that Uses (s, "x")
enum class TokenType {
    SPECIAL_CHARACTER, // (, ), +, -, *, /, %, ;, \n,
    // ', '
    // ", "
    // "x" and select is the same kind of token?
    WILDCARD, // _
    // procedure, stmt, read, print, assign, call, while, if, variable, constant
    STRING, // normal string token, procedure, stmt, assign, call, while, if, variable, constant, Select
    INTEGER, // statement number given in relationship arguments
    NAME, // NAME, has to start with LETTER variable name, procedure name, constant name, CAN BE ALPHANUMERIC
    CONSTANT_STRING, // within " " will refer to as constant string as they are immutable and shldnt be splitted up
};



class QueryToken {
    protected:
        TokenType type;
        std::string value;

    public:
        QueryToken(TokenType type, std::string value): type(type), value(std::move(value)) {}

        TokenType getType() const {
            return type;
        };

        std::string getTypeString() const {
            switch (type) {
                case TokenType::SPECIAL_CHARACTER:
                    return "SPECIAL_CHARACTER";
                case TokenType::WILDCARD:
                    return "WILDCARD";
                case TokenType::STRING:
                    return "STRING";
                case TokenType::INTEGER:
                    return "INTEGER";
                case TokenType::NAME:
                    return "NAME";
                case TokenType::CONSTANT_STRING:
                    return "CONSTANT_STRING";
            }
        }

        std::string getValue() const {
            return value;
        };

        bool operator==(QueryToken other) const {
            return type == other.getType() && value == other.getValue();
        };

        bool operator!=(QueryToken other) const {
            return type != other.getType() || value != other.getValue();
        };

        void print() const{
            std::cout << "Type: " << static_cast<int>(type) << " (" << getTypeString() << ")" <<  " Value: " << value << std::endl;
        }

};

