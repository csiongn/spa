#include "catch.hpp"
#include "QPS/QueryParser.h"
#include "QPS/QueryTokenizer.h"
#include "QPS/QuerySemanticError.h"
using namespace std;

TEST_CASE("Parse") {
    SECTION("Only select clause") {
        QueryTokenizer queryTokenizer{};
        std::string query = "variable v; \nSelect v";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("unused declarations") {
        QueryTokenizer queryTokenizer{};
        std::string query = "variable v; assign a; \nSelect v";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("declarations of same synonym identity") {
        QueryTokenizer queryTokenizer{};
        std::string query = "variable v; assign v; \nSelect v";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("Parent relationship") {
        QueryTokenizer queryTokenizer{};
        std::string query = "while w;\n"
                            "Select w such that Parent(w, 7)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::WHILE, "w");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::WHILE, "w");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::WHILE, "w");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT_NO, "7");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("Follows* relationship with wildcard as first argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s; \nSelect s such that Follows*(_, s)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT, "s");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }


    SECTION("Uses relationship with non-variable synonym as second argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s; variable v; \nSelect s such that Uses(6, s)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

        REQUIRE_THROWS_WITH(queryParser.parse(), "Semantic Error: Second argument to Modifies and Uses should be a variable synonym");
    }

    SECTION("Uses relationship with variable synonym as second argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s; variable v; \nSelect s such that Uses(6, v)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "6");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);

        REQUIRE(expectedQuery == results);
    }

    SECTION("Uses relationship with if synonym as first argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s; if if; \nSelect s such that Uses(if, \"x\")";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::IF, "if");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::IF, "if");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "x");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);

        REQUIRE(expectedQuery == results);
    }

    SECTION("Modifies relationship with while synonym as first argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s; while w; \nSelect s such that Uses(w, \"x\")";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::WHILE, "w");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::WHILE, "w");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "x");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);

        REQUIRE(expectedQuery == results);
    }

    SECTION("Uses relationship with ident as second argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s; variable v; \nSelect s such that Uses(6, \"x\")";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "6");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "x");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);

        REQUIRE(expectedQuery == results);
    }

    SECTION("Modifies relationship with IDENT as second argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "variable v; \nSelect v such that Modifies(7, \"v\")";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::VARIABLE, "v");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "7");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "v");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);

        REQUIRE(expectedQuery == results);
    }

    SECTION("Modifies relationship with IDENT as second argument and read as first argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "variable v; read re; \nSelect v such that Modifies(re, \"v\")";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::READ, "re");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::VARIABLE, "v");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::READ, "re");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::IDENT, "v");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);

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

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN, "a");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::ASSIGN, "a");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::USESS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);

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

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN, "a");

        PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
        PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::EXPR, "x");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg0);
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
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

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN, "a");

        PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
        PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::PARTIAL_EXPR, "_x_");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg0);
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
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

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN, "a");

        PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
        PQL::Synonym arg1(SimpleProgram::DesignEntity::WILDCARD, "_");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg0);
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
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
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN, "a");

        PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
        PQL::Synonym arg1(SimpleProgram::DesignEntity::VARIABLE, "v");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg0);
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);

    }

    SECTION("pattern clause non-variable synonym first argument") {
        QueryTokenizer queryTokenizer{};
        std::string query = "assign a; print pn;\nSelect a pattern a ( pn , _ )";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        REQUIRE_THROWS_WITH(queryParser.parse(), "Semantic Error: Synonym for first argument should be a variable synonym");
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

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN, "a");

        PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
        PQL::Synonym arg1(SimpleProgram::DesignEntity::IDENT, "v");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg0);
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);

    }

    SECTION("Uses relationship") {
        QueryTokenizer queryTokenizer{};
        std::string query = "variable v;\n"
                            "Select v such that Modifies(6, v)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::VARIABLE, "v");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "6");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::VARIABLE, "v");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::MODIFIESS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
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
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
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
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
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
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
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
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("Follows* relationship with both arguments as different stmt nos") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s;\n Select s such that Follows*(1, 3)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT_NO, "1");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT_NO, "3");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("Follows* relationship with both arguments as stmt synonym") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s;\n Select s such that Follows*(s, s)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT, "s");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT, "s");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("Follows* relationship with both arguments as stmt synonym using integer as declaration") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt 1;\n Select 1 such that Follows*(1, 1)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "1");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "1");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT, "1");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT, "1");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
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
        std::string query = "print stmt pn;\n Select pn such that Uses(pn, \"east\")";
        auto tokens = queryTokenizer.tokenize(query);

        QueryParser queryParser(tokens);

        REQUIRE_THROWS_WITH(queryParser.parse(), "Syntax Error: Invalid declaration syntax");
    }

    SECTION("multiple declarations") {
        QueryTokenizer queryTokenizer{};
        std::string query = "assign a1, a2; \nSelect a1 such that Follows(a1,a2)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a1");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a2");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN, "a1");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::ASSIGN, "a1");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::ASSIGN, "a2");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("multiple declarations 2") {
        QueryTokenizer queryTokenizer{};
        std::string query = "assign a1, a2;\nSelect a2 such that Follows(a1, a2)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a2");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a1");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::ASSIGN, "a2");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::ASSIGN, "a1");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::ASSIGN, "a2");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("Parent* relationship") {
        QueryTokenizer queryTokenizer{};
        std::string query = "stmt s1, s2;\n Select s1 such that Parent*(s1, s2)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s1");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s2");
        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s1");

        PQL::Synonym arg1(SimpleProgram::DesignEntity::STMT, "s1");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::STMT, "s2");
        std::vector<PQL::Synonym> args;
        args.emplace_back(arg1);
        args.emplace_back(arg2);
        PQL::Clause clause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, args);
        expectedClauses.emplace_back(clause);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("such that clause then pattern clause") {
        QueryTokenizer queryTokenizer{};
        std::string query = "assign a; variable v; stmt s1, s2;\n Select s1 such that Parent*(s1, s2) pattern a (v, _)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s1");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s2");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");

        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s1");

        PQL::Synonym parentArg1(SimpleProgram::DesignEntity::STMT, "s1");
        PQL::Synonym parentArg2(SimpleProgram::DesignEntity::STMT, "s2");
        std::vector<PQL::Synonym> parentArgs;
        parentArgs.emplace_back(parentArg1);
        parentArgs.emplace_back(parentArg2);
        PQL::Clause clause1 = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, parentArgs);
        expectedClauses.emplace_back(clause1);

        PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
        PQL::Synonym arg1(SimpleProgram::DesignEntity::VARIABLE, "v");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
        std::vector<PQL::Synonym> patternArgs;
        patternArgs.emplace_back(arg0);
        patternArgs.emplace_back(arg1);
        patternArgs.emplace_back(arg2);
        PQL::Clause clause2 = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, patternArgs);
        expectedClauses.emplace_back(clause2);


        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }

    SECTION("pattern clause then such that clause") {
        QueryTokenizer queryTokenizer{};
        std::string query = "assign a; variable v; stmt s1, s2; \n"
                            "Select s1 pattern a (v, _) such that Parent*(s1, s2)";
        auto tokens = queryTokenizer.tokenize(query);
        QueryParser queryParser(tokens);

        auto results = queryParser.parse();

        std::vector<PQL::Synonym> expectedDeclarations;
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s1");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::ASSIGN, "a");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::VARIABLE, "v");
        expectedDeclarations.emplace_back(SimpleProgram::DesignEntity::STMT, "s2");

        std::vector<PQL::Clause> expectedClauses;

        PQL::Synonym expectedSelectSynonym(SimpleProgram::DesignEntity::STMT, "s1");

        PQL::Synonym arg0(SimpleProgram::DesignEntity::ASSIGN, "a");
        PQL::Synonym arg1(SimpleProgram::DesignEntity::VARIABLE, "v");
        PQL::Synonym arg2(SimpleProgram::DesignEntity::WILDCARD, "_");
        std::vector<PQL::Synonym> patternArgs;
        patternArgs.emplace_back(arg0);
        patternArgs.emplace_back(arg1);
        patternArgs.emplace_back(arg2);
        PQL::Clause clause1 = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_ASSIGN, patternArgs);
        expectedClauses.emplace_back(clause1);

        PQL::Synonym parentArg1(SimpleProgram::DesignEntity::STMT, "s1");
        PQL::Synonym parentArg2(SimpleProgram::DesignEntity::STMT, "s2");
        std::vector<PQL::Synonym> parentArgs;
        parentArgs.emplace_back(parentArg1);
        parentArgs.emplace_back(parentArg2);
        PQL::Clause clause2 = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, parentArgs);
        expectedClauses.emplace_back(clause2);

        PQL::Query expectedQuery = PQL::Query(expectedDeclarations, expectedClauses, expectedSelectSynonym);
        REQUIRE(expectedQuery == results);
    }
}


