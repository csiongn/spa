#pragma once

#include "AST.h"
#include "Token.h"

class Parser {
private:
    std::vector<Token> tokens;
    // Index of current token to be parsed
    size_t current = 0;
    uint16_t stmtNumber = 0;

    bool match(const std::vector<SP::TokenType>& types);
    Token consume(SP::TokenType type, const std::string& message);
    bool check(SP::TokenType type);
    Token advance();
    bool isAtEnd();

    void throwParseError(const std::string& message);

    // Expression parsing
    std::unique_ptr<ExprNode> expression();
    std::unique_ptr<ExprNode> term();
    std::unique_ptr<ExprNode> factor();
    std::unique_ptr<ExprNode> condExpr();
    std::unique_ptr<ExprNode> relExpr();
    std::unique_ptr<ExprNode> negExpr();

    // Statement parsing
    std::unique_ptr<StmtNode> parseStatement();
    std::unique_ptr<ProgramNode> parseProgram();
    std::unique_ptr<ProcedureNode> parseProcedure();
    std::unique_ptr<CallNode> parseCall();
    std::unique_ptr<BlockNode> parseBlock();
    std::unique_ptr<ReadNode> parseRead();
    std::unique_ptr<PrintNode> parsePrint();
    std::unique_ptr<AssignNode> parseAssign();
    std::unique_ptr<WhileNode> parseWhile();
    std::unique_ptr<IfNode> parseIf();

public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
    std::unique_ptr<ProgramNode> parse();
};
