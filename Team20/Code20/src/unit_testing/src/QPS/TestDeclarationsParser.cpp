#include "../lib/catch.hpp"
#include "QPS/Parser/DeclarationsParser.h"
#include "QPS/QueryTokenizer.h"
#include "QPS/QuerySyntaxError.h"

TEST_CASE("Declarations Parser") {
    QueryTokenizer tokenizer{};
    std::string singleDeclarationsQuery = R"(assign a; constant c;)";
    std::string doubleDeclarationsQuery = R"(assign a1, a2; constant c1, c2;)";

    SECTION("Parse single declarations") {
        SECTION("Valid declarations") {
            std::vector<std::shared_ptr<QueryToken>> singleDeclarationTokens = tokenizer.tokenize(singleDeclarationsQuery);
            DeclarationsParser parser(singleDeclarationTokens, std::vector{2, 5});
            std::vector<PQL::Synonym> declarations = parser.parseDeclarations();
            REQUIRE_NOTHROW(declarations);

            std::vector<PQL::Synonym> expectedDeclarations{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a"),
                                                           PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c")};
            REQUIRE(declarations == expectedDeclarations);
        }

        SECTION("Invalid declaration type") {
            DeclarationsParser parser(tokenizer.tokenize(R"(assign a; c c1;)"), std::vector{2, 5});
            REQUIRE_THROWS_AS(parser.parseDeclarations(), QuerySyntaxError);
        }
    }

    SECTION("Parse multiple declarations") {
        SECTION("Valid declarations") {
            std::vector<std::shared_ptr<QueryToken>> singleDeclarationTokens = tokenizer.tokenize(doubleDeclarationsQuery);
            DeclarationsParser parser(singleDeclarationTokens, std::vector{4, 9});
            std::vector<PQL::Synonym> declarations = parser.parseDeclarations();
            REQUIRE_NOTHROW(declarations);

            std::vector<PQL::Synonym> expectedDeclarations{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a1"),
                                                           PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a2"),
                                                           PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c1"),
                                                           PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c2")};
            REQUIRE(declarations == expectedDeclarations);
        }

        SECTION("Invalid declaration type") {
            DeclarationsParser parser(tokenizer.tokenize(R"(assign a, a1; c c1, c2;)"), std::vector{4, 9});
            REQUIRE_THROWS_AS(parser.parseDeclarations(), QuerySyntaxError);
        }
        
        SECTION("Multiple declarations of the same name") {
            DeclarationsParser parser(tokenizer.tokenize(R"(assign a, a;)"), std::vector{4});
            REQUIRE_THROWS_AS(parser.parseDeclarations(), QuerySyntaxError);
        }
    }
}