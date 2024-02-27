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
    Token consume(const std::vector<SP::TokenType>& types, const std::string& message);
    bool check(SP::TokenType type);
    Token advance();
    bool isAtEnd();
    bool isKeyword();

    void throwParseError(const std::string& message);

    // Expression parsing
    std::shared_ptr<ExprNode> expression();
    std::shared_ptr<ExprNode> term();
    std::shared_ptr<ExprNode> factor();
    std::shared_ptr<ExprNode> condExpr();
    std::shared_ptr<ExprNode> relExpr();
    std::shared_ptr<ExprNode> negExpr();

    // Statement parsing
    std::shared_ptr<StmtNode> parseStatement();
    std::shared_ptr<ProgramNode> parseProgram();
    std::shared_ptr<ProcedureNode> parseProcedure();
    std::shared_ptr<CallNode> parseCall();
    std::shared_ptr<BlockNode> parseBlock();
    std::shared_ptr<ReadNode> parseRead();
    std::shared_ptr<PrintNode> parsePrint();
    std::shared_ptr<AssignNode> parseAssign();
    std::shared_ptr<WhileNode> parseWhile();
    std::shared_ptr<IfNode> parseIf();

public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
    std::shared_ptr<ProgramNode> parse();
};
