#include <iostream>
#include <string>
#include <vector>

#include "Parser.h"
#include "AST.h"

// Utilities

// Checks if current token type in parser is in list of types, and advances if found
bool Parser::match(const std::vector<SP::TokenType>& types) {
	for (SP::TokenType type : types) {
		if (check(type)) {
			advance();
			return true;
		}
	}
	return false;
}

Token Parser::consume(SP::TokenType type, const std::string& message) {
	if (check(type)) return advance();
	throwParseError(message);
}

// Checks if current token type in parser in of type type
bool Parser::check(SP::TokenType type) {
	if (isAtEnd()) return false;
	return tokens[current].type == type;
}

// Returns the current token and advances the parser to the next token
Token Parser::advance() {
	if (!isAtEnd()) current++;
	return tokens[current - 1];
}

bool Parser::isAtEnd() {
	return tokens[current].type == SP::TokenType::EOFILE;
}

// Error handler

void Parser::throwParseError(const std::string& message) {
	std::cerr << "Encountered error while parsing token " << current + 1 << ": '" << tokens[current].value << "'" << std::endl;
	std::cerr << message << std::endl;
	throw std::invalid_argument(message);
}

// Parser

std::unique_ptr<ProgramNode> Parser::parse() {
	// Start parsing from the highest-level rule.
	return parseProgram();
}

// Expression parsing

std::unique_ptr<ExprNode> Parser::expression() {
	std::unique_ptr<ExprNode> expr = term();

	while (match({SP::TokenType::PLUS, SP::TokenType::MINUS})) {
		std::string op = tokens[current - 1].value;
		std::unique_ptr<ExprNode> right = term();
		expr = std::make_unique<BinaryExprNode>(std::move(expr), op, std::move(right));
	}

	return expr;
}

std::unique_ptr<ExprNode> Parser::term() {
	std::unique_ptr<ExprNode> expr = factor();

	while (match({SP::TokenType::TIMES, SP::TokenType::SLASH, SP::TokenType::PERCENT})) {
		std::string op = tokens[current - 1].value;
		std::unique_ptr<ExprNode> right = factor();
		expr = std::make_unique<BinaryExprNode>(std::move(expr), op, std::move(right));
	}

	return expr;
}

std::unique_ptr<ExprNode> Parser::factor() {
	if (match({SP::TokenType::LEFT_PAREN})) {
		std::unique_ptr<ExprNode> expr = expression();
		consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after expression.");
		return expr;
	} else if (match({SP::TokenType::INTEGER, SP::TokenType::NAME})) {
		// Assuming the last matched token is the one we're interested in
		// For NAME, it could represent a variable access; for INTEGER, a literal value
		std::string value = tokens[current - 1].value;
		if (tokens[current - 1].type == SP::TokenType::INTEGER) {
			return std::make_unique<LiteralNode>(value);
		} else {
			return std::make_unique<VariableNode>(value);
		}
	}
	throwParseError("Expected '(' or integer or name when parsing factor");
}

std::unique_ptr<ExprNode> Parser::condExpr() {
	if (check({SP::TokenType::NOT})) {
		return negExpr();
	}

	std::unique_ptr<ExprNode> expr;

	if (match({SP::TokenType::LEFT_PAREN})) {
		expr = condExpr();
		consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after expression.");

		while (match({SP::TokenType::AND, SP::TokenType::OR})) {
			if (check({SP::TokenType::NOT})) {
				return negExpr();
			}
			std::string op = tokens[current - 1].value;
			consume(SP::TokenType::LEFT_PAREN, "Expected '(' after '&&' or '||'.");
			std::unique_ptr<ExprNode> left = condExpr();
			consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after expression.");
			expr = std::make_unique<LogicalOpNode>(std::move(left), op, std::move(expr));
		}
	} else {
		expr = relExpr();
	}
	return expr;
}

std::unique_ptr<ExprNode> Parser::negExpr() {
	if (match({SP::TokenType::NOT})) {
		consume(SP::TokenType::LEFT_PAREN, "Expected '(' after '!'.");
		std::unique_ptr<ExprNode> expr = condExpr();
		consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after expression.");
		return std::make_unique<NegationNode>(std::move(expr));
	}
}

std::unique_ptr<ExprNode> Parser::relExpr() {
	std::unique_ptr<ExprNode> left = expression();

	if (match({SP::TokenType::GREATER_THAN, SP::TokenType::GREATER_THAN_EQUAL, SP::TokenType::LESS_THAN, SP::TokenType::LESS_THAN_EQUAL, SP::TokenType::DOUBLE_EQUAL, SP::TokenType::NOT_EQUAL})) {
		std::string relational_op = tokens[current - 1].value;

		std::unique_ptr<ExprNode> right = expression();

		left = std::make_unique<RelExprNode>(std::move(left), relational_op, std::move(right));

		return left;
	}

	throwParseError("Expected relative operator when parsing relative expression.");

}

// Statement parsing

std::unique_ptr<ProgramNode> Parser::parseProgram() {
	std::vector<std::unique_ptr<ProcedureNode>> procedures;
	while (!isAtEnd()) {
		if (match({SP::TokenType::KEYWORD_PROCEDURE})) {
			procedures.push_back(parseProcedure());
		} else {
			throwParseError("Expected keyword procedure when parsing program");
		}
	}
	return std::make_unique<ProgramNode>(std::move(procedures));
}

std::unique_ptr<ProcedureNode> Parser::parseProcedure() {
	Token name = consume(SP::TokenType::NAME, "Expect procedure name.");
	std::unique_ptr<BlockNode> body = parseBlock();
	// No need for RIGHT_BRACE as parseBlock consumes it

	return std::make_unique<ProcedureNode>(name.value, std::move(body));
}

std::unique_ptr<StmtNode> Parser::parseStatement() {
	if (match({SP::TokenType::KEYWORD_CALL})) return parseCall();
	if (match({SP::TokenType::KEYWORD_PRINT})) return parsePrint();
	if (match({SP::TokenType::KEYWORD_READ})) return parseRead();
	if (check(SP::TokenType::NAME) && tokens[current + 1].type == SP::TokenType::EQUAL) return parseAssign();
	// parseAssign does not use match (which automatically consumes the checked token and advances) as
	// the assigned variable is important
	if (match({SP::TokenType::KEYWORD_IF})) return parseIf();
	if (match({SP::TokenType::KEYWORD_WHILE})) return parseWhile();
	throwParseError("Expected statement keyword or identifier when parsing statement");
}

std::unique_ptr<CallNode> Parser::parseCall() {
	Token procNameToken = consume(SP::TokenType::NAME, "Expect procedure name.");
	consume(SP::TokenType::SEMICOLON, "Expect ';' after procedure call.");

	return std::make_unique<CallNode>(++stmtNumber, procNameToken.value);
}

std::unique_ptr<ReadNode> Parser::parseRead() {
	Token name = consume(SP::TokenType::NAME, "Expect variable name after 'read'.");
	consume(SP::TokenType::SEMICOLON, "Expect ';' after variable name.");
	return std::make_unique<ReadNode>(++stmtNumber, name.value);
}

std::unique_ptr<PrintNode> Parser::parsePrint() {
	Token name = consume(SP::TokenType::NAME, "Expect variable name after 'print'.");
	consume(SP::TokenType::SEMICOLON, "Expect ';' after variable name.");
	return std::make_unique<PrintNode>(++stmtNumber, name.value);
}

std::unique_ptr<AssignNode> Parser::parseAssign() {
	Token varName = consume(SP::TokenType::NAME, "Expect variable name.");
	consume(SP::TokenType::EQUAL, "Expect '=' after variable name.");
	std::unique_ptr<ExprNode> value = expression();
	consume(SP::TokenType::SEMICOLON, "Expect ';' after expression.");

	return std::make_unique<AssignNode>(++stmtNumber, varName.value, std::move(value));
}

// Helper method for parsing a block of statements enclosed in braces
std::unique_ptr<BlockNode> Parser::parseBlock() {
	consume(SP::TokenType::LEFT_BRACE, "Expect '{' before block.");
	std::vector<std::unique_ptr<StmtNode>> statements;

	while (!check(SP::TokenType::RIGHT_BRACE) && !isAtEnd()) {
		statements.push_back(parseStatement());
	}

	consume(SP::TokenType::RIGHT_BRACE, "Expect '}' after block.");
	return std::make_unique<BlockNode>(std::move(statements));
}

std::unique_ptr<IfNode> Parser::parseIf() {
	uint16_t stmtNum = ++stmtNumber;
	consume(SP::TokenType::LEFT_PAREN, "Expect '(' after 'if' while parsing if block.");
	std::unique_ptr<ExprNode> condition = condExpr();
	consume(SP::TokenType::RIGHT_PAREN, "Expect ')' after condition while parsing if block.");

	consume(SP::TokenType::KEYWORD_THEN, "Expect 'then' after condition while parsing if block.");
	std::unique_ptr<BlockNode> thenBranch = parseBlock();

	std::unique_ptr<BlockNode> elseBranch = nullptr;
	if (match({SP::TokenType::KEYWORD_ELSE})) {
		elseBranch = parseBlock();
	}

	return std::make_unique<IfNode>(stmtNum, std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<WhileNode> Parser::parseWhile() {
	uint16_t stmtNum = ++stmtNumber;
	consume(SP::TokenType::LEFT_PAREN, "Expected '(' after 'while' while parsing while block.");
	std::unique_ptr<ExprNode> condition = condExpr();
	consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after condition while parsing while block.");

	std::unique_ptr<BlockNode> body = parseBlock();

	return std::make_unique<WhileNode>(stmtNum, std::move(condition), std::move(body));
}
