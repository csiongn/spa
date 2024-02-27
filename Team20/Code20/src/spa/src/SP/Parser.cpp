#include <iostream>
#include <string>
#include <vector>

#include "Parser.h"
#include "AST.h"
#include "Token.h"

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

Token Parser::consume(const std::vector<SP::TokenType>& types, const std::string& message) {
	for (SP::TokenType type : types) {
		if (check(type)) {
			return consume(type, message);
		}
	}
	throwParseError(message);
}

// Checks if current token type in parser in of type type
bool Parser::check(SP::TokenType type) {
	if (isAtEnd()) return false;
	return tokens[current].type == type ||
		(type == SP::TokenType::NAME && isKeyword()); // Allow token type to be keyword when we expect a name token
}

// Returns the current token and advances the parser to the next token
Token Parser::advance() {
	if (!isAtEnd()) current++;
	return tokens[current - 1];
}

bool Parser::isAtEnd() {
	return tokens[current].type == SP::TokenType::EOFILE;
}

bool Parser::isKeyword() {
	// Retrieve the current token type
	SP::TokenType type = tokens[current].type;

	// Find the current token type in the keywords vector
	auto it = std::find(keywords.begin(), keywords.end(), type);

	// If the iterator is not at the end, the token type was found in the keywords vector
	return it != keywords.end();
}

// Error handler

void Parser::throwParseError(const std::string& message) {
	std::cerr << "Encountered error while parsing token " << current + 1 << ": '" << tokens[current].value << "'" << std::endl;
	std::cerr << message << std::endl;
	throw std::invalid_argument(message);
}

// Parser

std::shared_ptr<ProgramNode> Parser::parse() {
	// Start parsing from the highest-level rule.
	return parseProgram();
}

// Expression parsing

std::shared_ptr<ExprNode> Parser::expression() {
	std::shared_ptr<ExprNode> expr = term();

	while (match({SP::TokenType::PLUS, SP::TokenType::MINUS})) {
		std::string op = tokens[current - 1].value;
		std::shared_ptr<ExprNode> right = term();
		expr = std::make_shared<BinaryExprNode>(std::move(expr), op, std::move(right));
	}

	return expr;
}

std::shared_ptr<ExprNode> Parser::term() {
	std::shared_ptr<ExprNode> expr = factor();

	while (match({SP::TokenType::TIMES, SP::TokenType::SLASH, SP::TokenType::PERCENT})) {
		std::string op = tokens[current - 1].value;
		std::shared_ptr<ExprNode> right = factor();
		expr = std::make_shared<BinaryExprNode>(std::move(expr), op, std::move(right));
	}

	return expr;
}

std::shared_ptr<ExprNode> Parser::factor() {
	if (match({SP::TokenType::LEFT_PAREN})) {
		std::shared_ptr<ExprNode> expr = expression();
		consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after expression.");
		return expr;
	} else if (match({SP::TokenType::INTEGER, SP::TokenType::NAME})) {
		// Assuming the last matched token is the one we're interested in
		// For NAME, it could represent a variable access; for INTEGER, a literal value
		std::string value = tokens[current - 1].value;
		if (tokens[current - 1].type == SP::TokenType::INTEGER) {
			return std::make_shared<LiteralNode>(value);
		} else {
			return std::make_shared<VariableNode>(value);
		}
	}
	throwParseError("Expected '(' or integer or name when parsing factor");
}

std::shared_ptr<ExprNode> Parser::condExpr() {
	if (check({SP::TokenType::NOT})) {
		return negExpr();
	}

	if (!check({SP::TokenType::LEFT_PAREN})) {
		return relExpr();
	}

	std::shared_ptr<ExprNode> expr;

	if (match({SP::TokenType::LEFT_PAREN})) {
		expr = condExpr();
		consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after expression.");

		consume({SP::TokenType::AND, SP::TokenType::OR},
			"Expected logical operator && or || when parsing conditional expressions with parentheses");

		std::string op = tokens[current - 1].value;
		std::shared_ptr<ExprNode> right;
		consume(SP::TokenType::LEFT_PAREN, "Expected '(' after '&&' or '||'.");
		right = condExpr();
		consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after expression.");
		expr = std::make_shared<LogicalOpNode>(std::move(expr), op, std::move(right));
	}

	return expr;
}

std::shared_ptr<ExprNode> Parser::negExpr() {
	if (match({SP::TokenType::NOT})) {
		consume(SP::TokenType::LEFT_PAREN, "Expected '(' after '!'.");
		std::shared_ptr<ExprNode> expr = condExpr();
		consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after expression.");
		return std::make_shared<NegationNode>(std::move(expr));
	}
}

std::shared_ptr<ExprNode> Parser::relExpr() {
	std::shared_ptr<ExprNode> left = expression();

	if (match({SP::TokenType::GREATER_THAN, SP::TokenType::GREATER_THAN_EQUAL, SP::TokenType::LESS_THAN, SP::TokenType::LESS_THAN_EQUAL, SP::TokenType::DOUBLE_EQUAL, SP::TokenType::NOT_EQUAL})) {
		std::string relational_op = tokens[current - 1].value;

		std::shared_ptr<ExprNode> right = expression();

		left = std::make_shared<RelExprNode>(std::move(left), relational_op, std::move(right));

		return left;
	}

	throwParseError("Expected relative operator when parsing relative expression.");

}

// Statement parsing

std::shared_ptr<ProgramNode> Parser::parseProgram() {
	std::vector<std::shared_ptr<ProcedureNode>> procedures;
	while (!isAtEnd()) {
		if (match({SP::TokenType::KEYWORD_PROCEDURE})) {
			procedures.push_back(parseProcedure());
		} else {
			throwParseError("Expected keyword procedure when parsing program");
		}
	}
	return std::make_shared<ProgramNode>(std::move(procedures));
}

std::shared_ptr<ProcedureNode> Parser::parseProcedure() {
	Token name = consume(SP::TokenType::NAME, "Expect procedure name.");
	std::shared_ptr<BlockNode> body = parseBlock();
	// No need for RIGHT_BRACE as parseBlock consumes it

	return std::make_shared<ProcedureNode>(name.value, std::move(body));
}

std::shared_ptr<StmtNode> Parser::parseStatement() {
	// Look-ahead to determine whether current token is being used as name or keyword
	if (isKeyword() && tokens[current + 1].type == SP::TokenType::EQUAL) {
		// The keyword is being used as a variable name in an assignment
		return parseAssign();
	}
	// parseAssign does not use match (which automatically consumes the checked token and advances) as
	// the assigned variable is important
	if (check(SP::TokenType::NAME) && tokens[current + 1].type == SP::TokenType::EQUAL) return parseAssign();
	if (match({SP::TokenType::KEYWORD_CALL})) return parseCall();
	if (match({SP::TokenType::KEYWORD_PRINT})) return parsePrint();
	if (match({SP::TokenType::KEYWORD_READ})) return parseRead();
	if (match({SP::TokenType::KEYWORD_IF})) return parseIf();
	if (match({SP::TokenType::KEYWORD_WHILE})) return parseWhile();
	throwParseError("Expected statement keyword or identifier when parsing statement");
}

std::shared_ptr<CallNode> Parser::parseCall() {
	Token procNameToken = consume(SP::TokenType::NAME, "Expect procedure name.");
	consume(SP::TokenType::SEMICOLON, "Expect ';' after procedure call.");

	return std::make_shared<CallNode>(++stmtNumber, procNameToken.value);
}

std::shared_ptr<ReadNode> Parser::parseRead() {
	Token name = consume(SP::TokenType::NAME, "Expect variable name after 'read'.");
	consume(SP::TokenType::SEMICOLON, "Expect ';' after variable name.");
	return std::make_shared<ReadNode>(++stmtNumber, name.value);
}

std::shared_ptr<PrintNode> Parser::parsePrint() {
	Token name = consume(SP::TokenType::NAME, "Expect variable name after 'print'.");
	consume(SP::TokenType::SEMICOLON, "Expect ';' after variable name.");
	return std::make_shared<PrintNode>(++stmtNumber, name.value);
}

std::shared_ptr<AssignNode> Parser::parseAssign() {
	Token varName = consume(SP::TokenType::NAME, "Expect variable name.");
	consume(SP::TokenType::EQUAL, "Expect '=' after variable name.");
	std::shared_ptr<ExprNode> value = expression();
	consume(SP::TokenType::SEMICOLON, "Expect ';' after expression.");

	return std::make_shared<AssignNode>(++stmtNumber, varName.value, std::move(value));
}

// Helper method for parsing a block of statements enclosed in braces
std::shared_ptr<BlockNode> Parser::parseBlock() {
	consume(SP::TokenType::LEFT_BRACE, "Expect '{' before block.");
	std::vector<std::shared_ptr<StmtNode>> statements;

	while (!check(SP::TokenType::RIGHT_BRACE) && !isAtEnd()) {
		statements.push_back(parseStatement());
	}

	consume(SP::TokenType::RIGHT_BRACE, "Expect '}' after block.");
	return std::make_shared<BlockNode>(std::move(statements));
}

std::shared_ptr<IfNode> Parser::parseIf() {
	uint16_t stmtNum = ++stmtNumber;
	consume(SP::TokenType::LEFT_PAREN, "Expect '(' after 'if' keyword while parsing if statement.");
	std::shared_ptr<ExprNode> condition = condExpr();
	consume(SP::TokenType::RIGHT_PAREN, "Expect ')' after condition while parsing if statement.");

	consume(SP::TokenType::KEYWORD_THEN, "Expect 'then' keyword after condition while parsing if statement.");
	std::shared_ptr<BlockNode> thenBranch = parseBlock();

	consume(SP::TokenType::KEYWORD_ELSE, "Expect 'else' keyword after then block while parsing if statement.");
	std::shared_ptr<BlockNode> elseBranch = parseBlock();

	return std::make_shared<IfNode>(stmtNum, std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::shared_ptr<WhileNode> Parser::parseWhile() {
	uint16_t stmtNum = ++stmtNumber;
	consume(SP::TokenType::LEFT_PAREN, "Expected '(' after 'while' while parsing while block.");
	std::shared_ptr<ExprNode> condition = condExpr();
	consume(SP::TokenType::RIGHT_PAREN, "Expected ')' after condition while parsing while block.");

	std::shared_ptr<BlockNode> body = parseBlock();

	return std::make_shared<WhileNode>(stmtNum, std::move(condition), std::move(body));
}
