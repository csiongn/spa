#include <map>
#include <iostream>

#include "catch.hpp"
#include "PKB/PKB.h"
#include "SP/Parser.h"
#include "SP/DesignExtractor.h"

TEST_CASE("Parser correctly parses procedure with lecture example", "[parser]") {
    GIVEN("A parser with a valid input sequence") {

        /*    SIMPLE Code
         *
         *    procedure main {
         * 1.    x = v * y + z * t;
         * 2.    if ((x > 2) || (y == 1) && !(x == 0)) then {
         * 3.       x = v * y + z * t;
         * 4.       x = v * y + z * t;
         * 5.       x = v * y + z * t;
         * 6.       if (x>2) then {
         * 7.          y = x;
         * 8.          x = y;
         * 9.          z = y;
         *           } else {
         * 10.         x = t;
         *           }
         *       } else {
         * 11.       x = t;
         *       }
         * 12.   y = x;
         *    }
         */
        std::vector<Token> tokens = {
            // procedure main {
            Token(SP::TokenType::KEYWORD_PROCEDURE, "procedure"),
            Token(SP::TokenType::NAME, "main"),
            Token(SP::TokenType::LEFT_BRACE, "{"),

            // 1. x = v + x * y + z * t;
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "v"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::TIMES, "*"),
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::NAME, "z"),
            Token(SP::TokenType::TIMES, "*"),
            Token(SP::TokenType::NAME, "t"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // 2. if ((x > 2) || (y == 1) && (x == 0)) then {
            Token(SP::TokenType::KEYWORD_IF, "if"),
            Token(SP::TokenType::LEFT_PAREN, "("),
            Token(SP::TokenType::LEFT_PAREN, "("),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::GREATER_THAN, ">"),
            Token(SP::TokenType::INTEGER, "0"),
            Token(SP::TokenType::RIGHT_PAREN, ")"),
            Token(SP::TokenType::OR, "||"),
            Token(SP::TokenType::LEFT_PAREN, "("),
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::DOUBLE_EQUAL, "=="),
            Token(SP::TokenType::INTEGER, "1"),
            Token(SP::TokenType::RIGHT_PAREN, ")"),
            Token(SP::TokenType::AND, "&&"),
            Token(SP::TokenType::NOT, "!"),
            Token(SP::TokenType::LEFT_PAREN, "("),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::DOUBLE_EQUAL, "=="),
            Token(SP::TokenType::INTEGER, "0"),
            Token(SP::TokenType::RIGHT_PAREN, ")"),
            Token(SP::TokenType::RIGHT_PAREN, ")"),
            Token(SP::TokenType::KEYWORD_THEN, "then"),
            Token(SP::TokenType::LEFT_BRACE, "{"),

            // 3. x = v + x * y + z * t;
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "v"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::TIMES, "*"),
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::NAME, "z"),
            Token(SP::TokenType::TIMES, "*"),
            Token(SP::TokenType::NAME, "t"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // 4. x = v + x * y + z * t;
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "v"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::TIMES, "*"),
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::NAME, "z"),
            Token(SP::TokenType::TIMES, "*"),
            Token(SP::TokenType::NAME, "t"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // 5. x = v + x * y + z * t;
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "v"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::TIMES, "*"),
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::PLUS, "+"),
            Token(SP::TokenType::NAME, "z"),
            Token(SP::TokenType::TIMES, "*"),
            Token(SP::TokenType::NAME, "t"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // 6. if (x > 2) then {
            Token(SP::TokenType::KEYWORD_IF, "if"),
            Token(SP::TokenType::LEFT_PAREN, "("),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::GREATER_THAN, ">"),
            Token(SP::TokenType::INTEGER, "2"),
            Token(SP::TokenType::RIGHT_PAREN, ")"),
            Token(SP::TokenType::KEYWORD_THEN, "then"),
            Token(SP::TokenType::LEFT_BRACE, "{"),

            // 7. y = x;
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // 8. x = y;
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // 9. z = y;
            Token(SP::TokenType::NAME, "z"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // 10. } else {
            Token(SP::TokenType::RIGHT_BRACE, "}"),
            Token(SP::TokenType::KEYWORD_ELSE, "else"),
            Token(SP::TokenType::LEFT_BRACE, "{"),

            // 11. x = t;
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "t"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // }
            Token(SP::TokenType::RIGHT_BRACE, "}"),

            // 12. } else {
            Token(SP::TokenType::RIGHT_BRACE, "}"),
            Token(SP::TokenType::KEYWORD_ELSE, "else"),
            Token(SP::TokenType::LEFT_BRACE, "{"),

            // 13. x = t;
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "t"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // }
            Token(SP::TokenType::RIGHT_BRACE, "}"),

            // 12. y = x;
            Token(SP::TokenType::NAME, "y"),
            Token(SP::TokenType::EQUAL, "="),
            Token(SP::TokenType::NAME, "x"),
            Token(SP::TokenType::SEMICOLON, ";"),

            // }
            Token(SP::TokenType::RIGHT_BRACE, "}"),

            Token(SP::TokenType::EOFILE, "")
        };

        Parser parser(tokens);
        auto pkb = std::make_shared<PKB>();
        std::shared_ptr<PKBFacade> pkbFacade = pkb->pkbFacade;

        WHEN("The parser attempts to parse the expression") {
            auto root = parser.parse();

            THEN("It should not return nullptr") {
                REQUIRE(root != nullptr);
            }

            AND_THEN("It should be serialized correctly") {
                auto expected_serialization = "Program [Procedure main [Block [Assign-1 x [BinaryExpr [BinaryExpr [Variable v + BinaryExpr [Variable x * Variable y]] + BinaryExpr [Variable z * Variable t]]] If-2 [LogicalOp [LogicalOp [RelExpr [Variable x > Literal 0] || RelExpr [Variable y == Literal 1]] && Not [RelExpr [Variable x == Literal 0]]]] then [Block [Assign-3 x [BinaryExpr [BinaryExpr [Variable v + BinaryExpr [Variable x * Variable y]] + BinaryExpr [Variable z * Variable t]]] Assign-4 x [BinaryExpr [BinaryExpr [Variable v + BinaryExpr [Variable x * Variable y]] + BinaryExpr [Variable z * Variable t]]] Assign-5 x [BinaryExpr [BinaryExpr [Variable v + BinaryExpr [Variable x * Variable y]] + BinaryExpr [Variable z * Variable t]]] If-6 [RelExpr [Variable x > Literal 2]] then [Block [Assign-7 y [Variable x] Assign-8 x [Variable y] Assign-9 z [Variable y]]] else [Block [Assign-10 x [Variable t]]]]] else [Block [Assign-11 x [Variable t]]] Assign-12 y [Variable x]]]]";
                auto actual_serialization = root->serialize();
                REQUIRE(expected_serialization == actual_serialization);
            }

            AND_WHEN("The design extractor extracts relationships") {
                auto extractor = DesignExtractor(pkbFacade);
                extractor.extractDesign(*root);

                THEN("The follows relationships should be correctly extracted") {
                    auto& follows = extractor.getFollows();
                    REQUIRE(follows.at(2) == 1); // Follows(1, 2)
                    REQUIRE(follows.at(4) == 3); // Follows(3, 4)
                    REQUIRE(follows.at(5) == 4); // Follows(4, 5)
                    REQUIRE(follows.at(12) == 2); // Follows(2, 12)
                    REQUIRE_THROWS(follows.at(1));   // Follows(s, 1) should not exist
                    REQUIRE_THROWS(follows.at(3));   // Follows(s, 3) should not exist
                }

                THEN("The transitive follows relationships should be correctly extracted") {
                    auto& followsT = extractor.getFollowsT();
                    auto& followsT_1 = followsT.at(1); // Get all statements that follow stmt 1 transitively
                    auto& followsT_3 = followsT.at(3); // Get all statements that follow stmt 3 transitively

                    REQUIRE(followsT_1.size() == 2); // two statement follows statement 1
                    REQUIRE(followsT_3.size() == 3); // three statements follow statement 3
                }
                THEN("The parents relationships should be correctly extracted") {
                    auto& parent = extractor.getParent();
                    REQUIRE(parent.at(3) == 2); // Parent(2, 3)
                    REQUIRE(parent.at(4) == 2); // Parent(2, 4)
                    REQUIRE(parent.at(5) == 2); // Parent(2, 5)
                    REQUIRE(parent.at(10) == 6); // Parent(6, 10)
                    REQUIRE_THROWS(parent.at(1));   // Parent(s, 1) should not exist
                }

                THEN("The transitive parent relationships should be correctly extracted") {
                    auto& parentT = extractor.getParentT();
                    auto& parentT_2 = parentT.at(2); // Get all statements that are the transitive children of statement 2
                    REQUIRE_THROWS(parentT.at(3)); // No statement is a transitive child of stmt 3
                    auto& parentT_6 = parentT.at(6);

                    REQUIRE(parentT_2.size() == 9); // nine statements are transitive children of statement 2
                    REQUIRE(parentT_6.size() == 4);
                }

                THEN("The variables should be correctly extracted") {
                    auto& variables = extractor.getVariables();
                    REQUIRE(variables.size() == 5);
                    REQUIRE(variables.count("x"));
                    REQUIRE(variables.count("y"));
                    REQUIRE(variables.count("z"));
                    REQUIRE(variables.count("v"));
                    REQUIRE(variables.count("t"));
                }
            }
        }
    }
}
