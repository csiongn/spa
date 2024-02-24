#include "catch.hpp"
#include "QPS/QueryParser.h"
#include "QPS/QueryTokenizer.h"
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
}


