#include "../lib/catch.hpp"
#include "QPS/Parser/QueryParser.h"
#include "QPS/QuerySemanticError.h"
#include "QPS/QuerySyntaxError.h"
#include "QPS/QueryTokenizer.h"

TEST_CASE("Parse") {
  SECTION("Only select clause") {
	QueryTokenizer queryTokenizer{};
	std::string query = "variable v; \nSelect v";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(
		SimpleProgram::DesignEntity::VARIABLE, "v");
	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("declarations of same synonym identity") {
	QueryTokenizer queryTokenizer{};
	std::string query = "variable v; assign v; \nSelect v";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);
	REQUIRE_THROWS_AS(queryParser.parse(), QuerySemanticError);
  }

  SECTION("Parent relationship") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"assign a;\n"
		"Select a such that Parent(a, 7)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT_NO, "7");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Follows* relationship with wildcard as first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "stmt s; \nSelect s such that Follows*(_, s)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT, "s");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Modifies relationship with non-variable synonym as second argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = R"(stmt s; variable v; Select s such that Modifies(6, s))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s");

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySemanticError);
  }

  SECTION("Uses relationship with variable synonym as second argument") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"stmt s; variable v; \nSelect s such that Uses(6, v)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "6");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::VARIABLE, "v");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("Uses relationship with if synonym as first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"stmt s; if if; \nSelect s such that Uses(if, \"x\")";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::IF,
									  "if");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::IF, "if");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "x");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("Modifies relationship with while synonym as first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"stmt s; while w; \nSelect s such that Uses(w, \"x\")";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::WHILE,
									  "w");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::WHILE, "w");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "x");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("Uses relationship with ident as second argument") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"stmt s; variable v; \nSelect s such that Uses(6, \"x\")";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "6");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "x");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("Modifies relationship with IDENT as second argument") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"variable v; \nSelect v such that Modifies(7, \"v\")";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(
		SimpleProgram::DesignEntity::VARIABLE, "v");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "7");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "v");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("Modifies relationship with IDENT as second argument and read as first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "variable v; read re; \nSelect v such that Modifies(re, \"v\")";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::READ,
									  "re");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(
		SimpleProgram::DesignEntity::VARIABLE, "v");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::READ, "re");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "v");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("Uses relationship with wildcard as second argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "assign a;\n Select a such that Uses(a, _)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause full expression") {
	QueryTokenizer queryTokenizer{};
	std::string query = "assign a; \nSelect a pattern a ( _ , \"x\" )";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::EXPR, "x");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause partial expression") {
	QueryTokenizer queryTokenizer{};
	std::string query = "assign a; \nSelect a pattern a ( _ , _\"x\"_ )";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_x_");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause double wildcard") {
	QueryTokenizer queryTokenizer{};
	std::string query = "assign a; \nSelect a pattern a ( _ , _ )";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause variable synonym first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "assign a; variable v;\nSelect a pattern a ( v , _ )";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::VARIABLE, "v");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause non-variable synonym first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "assign a; print pn;\nSelect a pattern a ( pn , _ )";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySemanticError);
  }

  SECTION("pattern clause ident first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "assign a;\nSelect a pattern a ( \"v\" , _ )";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::IDENT, "v");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Uses relationship") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"variable v;\n"
		"Select v such that Modifies(6, v)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::VARIABLE,
									   "v");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "6");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::VARIABLE, "v");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Follows* relationship with stmt synonym as first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "stmt s;\n Select s such that Follows*(s, 1)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT, "s");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT_NO, "1");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Follows* relationship with stmt synonym as second argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "stmt s;\n Select s such that Follows*(1, s)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "1");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT, "s");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Follows* relationship with stmt subtype synonym as first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = "call c;\n Select c such that Follows*(c, 1)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::CALL, "c");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::CALL, "c");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::CALL, "c");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT_NO, "1");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Follows* relationship with both arguments as stmt nos") {
	QueryTokenizer queryTokenizer{};
	std::string query = "stmt s;\n Select s such that Follows*(1, 1)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "1");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT_NO, "1");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses,
										  {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Follows* relationship with both arguments as different stmt nos") {
	QueryTokenizer queryTokenizer{};
	std::string query = "stmt s;\n Select s such that Follows*(1, 3)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "1");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT_NO, "3");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Follows* relationship with both arguments as stmt synonym") {
	QueryTokenizer queryTokenizer{};
	std::string query = "stmt s;\n Select s such that Follows*(s, s)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT, "s");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT, "s");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION(
	  "Follows* relationship with both arguments as stmt synonym using "
	  "integer as declaration") {
	QueryTokenizer queryTokenizer{};
	std::string query = "stmt 1;\n Select 1 such that Follows*(1, 1)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySyntaxError);
  }

  SECTION("Invalid select clause") {
	QueryTokenizer queryTokenizer{};
	std::string query = "variable v; \nselect v";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_WITH(queryParser.parse(), "Syntax Error: Select clause should come first");
  }

  SECTION("Invalid declaration syntax") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"print stmt pn;\n Select pn such that Uses(pn, \"east\")";
	auto tokens = queryTokenizer.tokenize(query);

	QueryParser queryParser(tokens);

	REQUIRE_THROWS_WITH(queryParser.parse(), "Syntax Error: Invalid declaration syntax");
  }

  SECTION("multiple declarations") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"assign a1, a2; \nSelect a1 such that Follows(a1,a2)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a1");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a2");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a1");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::ASSIGN, "a1");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::ASSIGN, "a2");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("multiple declarations 2") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"assign a1, a2;\nSelect a2 such that Follows(a1, a2)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a1");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a2");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a2");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::ASSIGN, "a1");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::ASSIGN, "a2");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("Parent* relationship") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"stmt s1, s2;\n Select s1 such that Parent*(s1, s2)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s1");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s2");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s1");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT, "s1");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT, "s2");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("such that clause then pattern clause") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"assign a; variable v; stmt s1, s2;\n Select s1 such that "
		"Parent*(s1, s2) pattern a (v, _)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s1");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s2");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s1");

	PQL::Synonym parentArg1(SimpleProgram::DesignEntity::STMT, "s1");
	PQL::Synonym parentArg2(SimpleProgram::DesignEntity::STMT, "s2");
	std::vector<PQL::Synonym> parentArgs;
	parentArgs.emplace_back(parentArg1);
	parentArgs.emplace_back(parentArg2);
	PQL::Clause clause1 =
		PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, parentArgs);
	expectedClauses.emplace_back(clause1);

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::VARIABLE, "v");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	std::vector<PQL::Synonym> patternArgs;
	patternArgs.emplace_back(arg0);
	patternArgs.emplace_back(arg1);
	patternArgs.emplace_back(arg2);
	PQL::Clause clause2 = PQL::Clause(
		SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, patternArgs);
	expectedClauses.emplace_back(clause2);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause then such that clause") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"assign a; variable v; stmt s1, s2; \n"
		"Select s1 pattern a (v, _) such that Parent*(s1, s2)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s1");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s2");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s1");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::VARIABLE, "v");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	std::vector<PQL::Synonym> patternArgs;
	patternArgs.emplace_back(arg0);
	patternArgs.emplace_back(arg1);
	patternArgs.emplace_back(arg2);
	PQL::Clause clause1 = PQL::Clause(
		SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, patternArgs);
	expectedClauses.emplace_back(clause1);

	PQL::Synonym parentArg1(SimpleProgram::DesignEntity::STMT, "s1");
	PQL::Synonym parentArg2(SimpleProgram::DesignEntity::STMT, "s2");
	std::vector<PQL::Synonym> parentArgs;
	parentArgs.emplace_back(parentArg1);
	parentArgs.emplace_back(parentArg2);
	PQL::Clause clause2 =
		PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, parentArgs);
	expectedClauses.emplace_back(clause2);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause with assign synonym as second declared") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		"assign a, a1; variable v; stmt s1, s2; \n"
		"Select s1 pattern a1 (v, _) such that Parent*(s1, s2)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a1");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE,
									  "v");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s1");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT,
									  "s2");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT,
									   "s1");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a1");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::VARIABLE, "v");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	std::vector<PQL::Synonym> patternArgs;
	patternArgs.emplace_back(arg0);
	patternArgs.emplace_back(arg1);
	patternArgs.emplace_back(arg2);
	PQL::Clause clause1 = PQL::Clause(
		SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, patternArgs);
	expectedClauses.emplace_back(clause1);

	PQL::Synonym parentArg1(SimpleProgram::DesignEntity::STMT, "s1");
	PQL::Synonym parentArg2(SimpleProgram::DesignEntity::STMT, "s2");
	std::vector<PQL::Synonym> parentArgs;
	parentArgs.emplace_back(parentArg1);
	parentArgs.emplace_back(parentArg2);
	PQL::Clause clause2 =
		PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, parentArgs);
	expectedClauses.emplace_back(clause2);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});
	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause with whitespace in expr") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(assign a; Select a such that Uses (a, "x") pattern a ("x", _"
  x"_))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym usesArg1(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym usesArg2(SimpleProgram::DesignEntity::IDENT, "x");
	std::vector<PQL::Synonym> usesArgs;
	usesArgs.emplace_back(usesArg1);
	usesArgs.emplace_back(usesArg2);
	PQL::Clause clause1 =
		PQL::Clause(SimpleProgram::DesignAbstraction::USESS, usesArgs);
	expectedClauses.emplace_back(clause1);

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::IDENT, "x");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_x_");
	std::vector<PQL::Synonym> patternArgs;
	patternArgs.emplace_back(arg0);
	patternArgs.emplace_back(arg1);
	patternArgs.emplace_back(arg2);
	PQL::Clause clause2 = PQL::Clause(
		SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, patternArgs);
	expectedClauses.emplace_back(clause2);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause with whitespace in expr") {
	QueryTokenizer queryTokenizer{};
	std::string query = R"(assign a; Select a pattern a (_, _"1 "_))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_1_");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern clause with whitespace in expr") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(assign a; Select a pattern a ("temp", _"temp "_))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::IDENT, "temp");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_temp_");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("assign synonym as Select") {
	QueryTokenizer queryTokenizer{};
	std::string query = R"(assign Select; Select Select)";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "Select");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "Select");
	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION(
	  "Follows* with non-statement synonym should throw QuerySyntaxError") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(stmt s, s1; Select s such that Follows*(s, "s1"))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySyntaxError);
  }

  SECTION(
	  "pattern clause with non expr or partial expr in second argument "
	  "should throw QuerySyntaxError") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(constant c; assign a; variable v; Select c pattern a (_,
  c))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySyntaxError);
  }

  SECTION(
	  "entRef ident to pattern clause first arg starting with digit should "
	  "throw QuerySyntaxError") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(assign a; variable v; constant c; Select a pattern a ("8",
  _))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySyntaxError);
  }SECTION(
	  "pattern assign clause expr invalid partial expr should throw "
	  "QuerySyntaxError") {
	QueryTokenizer queryTokenizer{};
	std::string query = R"(assign a; Select a pattern a (_, _"x"))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySyntaxError);
  }

  SECTION("pattern if clause") {
	QueryTokenizer queryTokenizer{};
	std::string query = R"(if ifs; Select ifs pattern ifs(_,_,_))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::IF,
									  "ifs");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::IF,
									   "ifs");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::IF, "ifs");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg3(SimpleProgram::DesignEntity::WILDCARD, "_");

	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	args.emplace_back(arg3);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern while clause") {
	QueryTokenizer queryTokenizer{};
	std::string query = R"(while w; Select w pattern w(_,_))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::WHILE,
									  "w");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::WHILE,
									   "w");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::WHILE, "w");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");

	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern if clause with while declaration") {
	QueryTokenizer queryTokenizer{};
	std::string query = R"(if ifs; while w; Select ifs pattern ifs(_,_,_))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();
	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::IF,
									  "ifs");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::WHILE,
									  "w");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::IF,
									   "ifs");

	PQL::Synonym arg0(SimpleProgram::DesignEntity::IF, "ifs");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym arg3(SimpleProgram::DesignEntity::WILDCARD, "_");

	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	args.emplace_back(arg3);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("pattern if clause with such that clause") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(if ifs; Select ifs pattern ifs(_,_,_) such that Follows(1,
  2))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::IF,
									  "ifs");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::IF,
									   "ifs");

	PQL::Synonym patternArg0(SimpleProgram::DesignEntity::IF, "ifs");
	PQL::Synonym patternArg1(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym patternArg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	PQL::Synonym patternArg3(SimpleProgram::DesignEntity::WILDCARD, "_");

	std::vector<PQL::Synonym> patternArgs;
	patternArgs.emplace_back(patternArg0);
	patternArgs.emplace_back(patternArg1);
	patternArgs.emplace_back(patternArg2);
	patternArgs.emplace_back(patternArg3);
	PQL::Clause patternClause = PQL::Clause(
		SimpleProgram::DesignAbstraction::PATTERN_IF, patternArgs);
	expectedClauses.emplace_back(patternClause);

	std::vector<PQL::Synonym> followsArgs;
	PQL::Synonym followsArg1(SimpleProgram::DesignEntity::STMT_NO, "1");
	PQL::Synonym followsArg2(SimpleProgram::DesignEntity::STMT_NO, "2");
	followsArgs.emplace_back(followsArg1);
	followsArgs.emplace_back(followsArg2);
	PQL::Clause followsClause =
		PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, followsArgs);
	expectedClauses.emplace_back(followsClause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("Follows relationship with variable as second argument") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(variable v; stmt s; Select s such that Follows* (s, v))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySemanticError);
  }

  SECTION("Follows relationship with variable as first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(variable v; stmt s; Select s such that Follows* (v, s))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	REQUIRE_THROWS_AS(queryParser.parse(), QuerySemanticError);
  }

  SECTION("pattern assign clause with such that parent clause") {
	QueryTokenizer queryTokenizer{};
	std::string query =
		R"(assign a, a1; Select a such that Parent* (28, a) pattern a1
  ("left", _))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a");
	expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN,
									  "a1");

	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN,
									   "a");

	PQL::Synonym suchThatArg1(SimpleProgram::DesignEntity::STMT_NO, "28");
	PQL::Synonym suchThatArg2(SimpleProgram::DesignEntity::ASSIGN, "a");
	std::vector<PQL::Synonym> suchThatArgs;
	suchThatArgs.emplace_back(suchThatArg1);
	suchThatArgs.emplace_back(suchThatArg2);
	PQL::Clause suchThatClause = PQL::Clause(
		SimpleProgram::DesignAbstraction::PARENTT, suchThatArgs);
	expectedClauses.emplace_back(suchThatClause);

	PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a1");
	PQL::Synonym arg1(SimpleProgram::DesignEntity::IDENT, "left");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");

	std::vector<PQL::Synonym> args;
	args.emplace_back(arg0);
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(expectedQuery == results);
  }

  SECTION("Modifies relationship with procedure synonym as first argument") {
	QueryTokenizer queryTokenizer{};
	std::string query = R"(procedure p; Select p such that Modifies(p, _))";
	auto tokens = queryTokenizer.tokenize(query);
	QueryParser queryParser(tokens);

	auto results = queryParser.parse();

	std::vector<PQL::Synonym> expectedDeclarations;
	expectedDeclarations.emplace_back(
		SimpleProgram::DesignEntity::PROCEDURE, "p");
	std::vector<PQL::Clause> expectedClauses;

	PQL::Synonym expectedSelectSynonym(
		SimpleProgram::DesignEntity::PROCEDURE, "p");

	PQL::Synonym arg1(SimpleProgram::DesignEntity::PROCEDURE, "p");
	PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
	std::vector<PQL::Synonym> args;
	args.emplace_back(arg1);
	args.emplace_back(arg2);
	PQL::Clause clause =
		PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESP, args);
	expectedClauses.emplace_back(clause);

	PQL::Query expectedQuery = PQL::Query(
		expectedDeclarations, expectedClauses, {expectedSelectSynonym});

	REQUIRE(results == expectedQuery);
  }
}