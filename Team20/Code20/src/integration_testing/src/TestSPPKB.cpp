#include <vector>
#include <string>
#include <memory>

#include "../lib/catch.hpp"

#include "../../unit_testing/src/TestUtils.h"
#include "SPA.h"

TEST_CASE("Integration test SP to PKB") {
  std::string testProgram = "procedure main {"
    "x = v * y + z * t;"
    "if ((x > 2) || ((y == 1) && (!(x == 0)))) then {"
    "x = v * y + z * t;"
    "y = 1 + 3 / 5 % x;"
    "if (x>2) then {"
    "x = 1;"
    "} else {"
    "a = (x % y) + ((y / 1) * 3);"
    "}"
    "} else {"
    "a = 5;"
    "}"
    "while (x < 2) {"
    "x = 1;"
    "}"
    "}"
    "procedure avg {"
    "avg = num1 + num2;"
    "avg = avg / 2;"
    "print avg;"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    SPFacade.populatePKB();
    THEN("The PKB should store the correct entities and variables") {
      // Check procedures
      std::vector<std::string> expectedProcs = {"main", "avg"};
      std::vector<std::string> actualProcs = pkbFacade->getAllProcedures();
      REQUIRE(checkVecValuesEqual(expectedProcs, actualProcs));

      // Check variables
      std::vector<std::string> expectedValues = {"a", "t", "v", "x", "y", "z", "avg", "num1", "num2"};
      std::vector<std::string> actualValues = pkbFacade->getAllVariables();
      REQUIRE(checkVecValuesEqual(expectedValues, actualValues));

      // Check literals
      std::vector<int> expectedliterals = {0, 1, 2, 3, 5};
      std::vector<int> actualLiterals = pkbFacade->getAllConstants();
      REQUIRE(checkVecValuesEqual(expectedliterals, actualLiterals));

      // Check statements
      std::vector<int> expectedStmts = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
      std::vector<int> actualStmts = pkbFacade->getAllStatementNum();
      REQUIRE(checkVecValuesEqual(expectedStmts, actualStmts));

      // Check if statements
      std::vector<int> expectedIfStmts = {2, 5};
      std::vector<int> actualIfStmts = pkbFacade->getAllIfStmtNum();
      REQUIRE(checkVecValuesEqual(expectedIfStmts, actualIfStmts));

      // Check print statements
      std::vector<int> expectedPrintStmts = {13};
      std::vector<int> actualPrintStmts = pkbFacade->getAllPrintStmtNum();
      REQUIRE(checkVecValuesEqual(expectedPrintStmts, actualPrintStmts));

      // Check assignment statements
      std::vector<int> expectedAssignStmts = {1, 3, 4, 6, 7, 8, 10, 11, 12};
      std::vector<int> actualAssignStmts = pkbFacade->getAllAssignStmtNum();
      REQUIRE(checkVecValuesEqual(expectedAssignStmts, actualAssignStmts));

      // No read statements
      REQUIRE(pkbFacade->getAllReadStmtNum().empty());

      // No while statements
      std::vector<int> expectedWhileStmts = {9};
      std::vector<int> actualWhileStmts = pkbFacade->getAllWhileStmtNum();
      REQUIRE(checkVecValuesEqual(expectedWhileStmts, actualWhileStmts));
    }

    AND_THEN("The PKB should store the correct relationships") {
      // Check Next relationships
      REQUIRE(pkbFacade->containsNextRelationship(1, 2));
      REQUIRE(pkbFacade->containsNextRelationship(2, 3));
      REQUIRE(pkbFacade->containsNextRelationship(2, 8));
      REQUIRE(pkbFacade->containsNextRelationship(3, 4));
      REQUIRE(pkbFacade->containsNextRelationship(4, 5));
      REQUIRE(pkbFacade->containsNextRelationship(5, 6));
      REQUIRE(pkbFacade->containsNextRelationship(5, 7));
      REQUIRE(pkbFacade->containsNextRelationship(6, 9));
      REQUIRE(pkbFacade->containsNextRelationship(7, 9));
      REQUIRE(pkbFacade->containsNextRelationship(8, 9));
      REQUIRE(pkbFacade->containsNextRelationship(9, 10));
      REQUIRE(pkbFacade->containsNextRelationship(10, 9));
      REQUIRE(pkbFacade->containsNextRelationship(11, 12));
      REQUIRE(pkbFacade->containsNextRelationship(12, 13));
    }
  }
}

TEST_CASE("Integration test from SP Parser to PKB") {
  GIVEN("An array of tokens representing the simple program") {
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

      // 2. if ((x > 2) || ((y == 1) && (!(x == 0)))) then {
      Token(SP::TokenType::KEYWORD_IF, "if"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::GREATER_THAN, ">"),
      Token(SP::TokenType::INTEGER, "2"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::OR, "||"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::DOUBLE_EQUAL, "=="),
      Token(SP::TokenType::INTEGER, "1"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::AND, "&&"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NOT, "!"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::DOUBLE_EQUAL, "=="),
      Token(SP::TokenType::INTEGER, "0"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
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

      // 4. y = 1 + 3 / 5 % x;
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::INTEGER, "1"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::INTEGER, "3"),
      Token(SP::TokenType::SLASH, "/"),
      Token(SP::TokenType::INTEGER, "5"),
      Token(SP::TokenType::PERCENT, "%"),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // 5. if (x>2) then {
      Token(SP::TokenType::KEYWORD_IF, "if"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::GREATER_THAN, ">"),
      Token(SP::TokenType::INTEGER, "2"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::KEYWORD_THEN, ""),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 6. x=1;
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::INTEGER, "1"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // } else {
      Token(SP::TokenType::RIGHT_BRACE, "}"),
      Token(SP::TokenType::KEYWORD_ELSE, "else"),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 8. a = (x % y) + ((y / 1) * 3);
      Token(SP::TokenType::NAME, "a"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::PERCENT, "%"),
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::SLASH, "/"),
      Token(SP::TokenType::INTEGER, "1"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::TIMES, "*"),
      Token(SP::TokenType::INTEGER, "3"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // };
      Token(SP::TokenType::RIGHT_BRACE, "}"),

      // } else {
      Token(SP::TokenType::RIGHT_BRACE, "}"),
      Token(SP::TokenType::KEYWORD_ELSE, "else"),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 7. a = 5;
      Token(SP::TokenType::NAME, "a"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::INTEGER, "5"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // }
      Token(SP::TokenType::RIGHT_BRACE, "}"),

      // }
      Token(SP::TokenType::RIGHT_BRACE, "}"),

      // procedure main {
      Token(SP::TokenType::KEYWORD_PROCEDURE, "procedure"),
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 8. avg = num1 + num2;
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::NAME, "num1"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::NAME, "num2"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // 9. avg = avg / 2;
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::SLASH, "/"),
      Token(SP::TokenType::INTEGER, "2"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // 10. print avg;
      Token(SP::TokenType::KEYWORD_PRINT, "print"),
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // }
      Token(SP::TokenType::RIGHT_BRACE, "}"),

      Token(SP::TokenType::EOFILE, ""),
    };

    // Initialise components
    // SP parser
    Parser parser(tokens);
    // PKB
    auto db = std::make_shared<Database>();
    std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);

    WHEN("The parser attempts to parse the expression") {
      auto root = parser.parse();

      THEN("It should not return nullptr") {
        REQUIRE(root != nullptr);
      }

      AND_WHEN("The design extractor extracts relationships") {
        // SP extractor
        DesignExtractor extractor(pkbFacade);
        extractor.extractDesign(*root);
        THEN("The PKB should store the correct entities and variables") {
          // Check procedures
          std::vector<std::string> expectedProcs = {"main", "avg"};
          std::vector<std::string> actualProcs = pkbFacade->getAllProcedures();
          REQUIRE(checkVecValuesEqual(expectedProcs, actualProcs));

          // Check variables
          std::vector<std::string> expectedValues = {"a", "t", "v", "x", "y", "z", "avg", "num1", "num2"};
          std::vector<std::string> actualValues = pkbFacade->getAllVariables();
          REQUIRE(checkVecValuesEqual(expectedValues, actualValues));

          // Check literals
          std::vector<int> expectedliterals = {0, 1, 2, 3, 5};
          std::vector<int> actualLiterals = pkbFacade->getAllConstants();
          REQUIRE(checkVecValuesEqual(expectedliterals, actualLiterals));

          // Check statements
          std::vector<int> expectedStmts = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
          std::vector<int> actualStmts = pkbFacade->getAllStatementNum();
          REQUIRE(checkVecValuesEqual(expectedStmts, actualStmts));

          // Check if statements
          std::vector<int> expectedIfStmts = {2, 5};
          std::vector<int> actualIfStmts = pkbFacade->getAllIfStmtNum();
          REQUIRE(checkVecValuesEqual(expectedIfStmts, actualIfStmts));

          // Check print statements
          std::vector<int> expectedPrintStmts = {11};
          std::vector<int> actualPrintStmts = pkbFacade->getAllPrintStmtNum();
          REQUIRE(checkVecValuesEqual(expectedPrintStmts, actualPrintStmts));

          // Check assignment statements
          std::vector<int> expectedAssignStmts = {1, 3, 4, 6, 7, 8, 9, 10};
          std::vector<int> actualAssignStmts = pkbFacade->getAllAssignStmtNum();
          REQUIRE(checkVecValuesEqual(expectedAssignStmts, actualAssignStmts));

          // No read statements
          REQUIRE(pkbFacade->getAllReadStmtNum().empty());

          // No while statements
          REQUIRE(pkbFacade->getAllWhileStmtNum().empty());
        }
      }
    }
  }
}

TEST_CASE("Integration test SP to PKB with keywords used as name") {
  std::string testProgram = "procedure test {"
    "if (less < b + c * d) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "while = 10;"
    "then = while * 3;"
    "if (1 < more) then {"
    "if = then * 5;"
    "} else {"
    "else = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    SPFacade.populatePKB();
    THEN("The PKB should store the correct entities and variables") {
      // Check procedures
      std::vector<std::string> expectedProcs = {"test"};
      std::vector<std::string> actualProcs = pkbFacade->getAllProcedures();
      REQUIRE(checkVecValuesEqual(expectedProcs, actualProcs));

      // Check variables
      std::vector<std::string> expectedValues = {
        "hello", "world", "less", "b", "c", "d", "more",
        "if", "then", "else", "while"
      };
      std::vector<std::string> actualValues = pkbFacade->getAllVariables();
      REQUIRE(checkVecValuesEqual(expectedValues, actualValues));

      // Check literals
      std::vector<int> expectedliterals = {1, 3, 5, 10};
      std::vector<int> actualLiterals = pkbFacade->getAllConstants();
      REQUIRE(checkVecValuesEqual(expectedliterals, actualLiterals));

      // Check statements
      std::vector<int> expectedStmts = {1, 2, 3, 4, 5, 6, 7, 8};
      std::vector<int> actualStmts = pkbFacade->getAllStatementNum();
      REQUIRE(checkVecValuesEqual(expectedStmts, actualStmts));

      // Check if statements
      std::vector<int> expectedIfStmts = {1, 6};
      std::vector<int> actualIfStmts = pkbFacade->getAllIfStmtNum();
      REQUIRE(checkVecValuesEqual(expectedIfStmts, actualIfStmts));

      // Check print statements
      std::vector<int> expectedPrintStmts = {3};
      std::vector<int> actualPrintStmts = pkbFacade->getAllPrintStmtNum();
      REQUIRE(checkVecValuesEqual(expectedPrintStmts, actualPrintStmts));

      // Check assignment statements
      std::vector<int> expectedAssignStmts = {2, 4, 5, 7, 8};
      std::vector<int> actualAssignStmts = pkbFacade->getAllAssignStmtNum();
      REQUIRE(checkVecValuesEqual(expectedAssignStmts, actualAssignStmts));

      // No read statements
      REQUIRE(pkbFacade->getAllReadStmtNum().empty());

      // No while statements
      REQUIRE(pkbFacade->getAllWhileStmtNum().empty());
    }
  }
}

TEST_CASE("Integration test SP to PKB with invalid extraneous brackets surrounding cond_expr") {
  std::string testProgram = "procedure test {"
    "if ((less < b + c * d)) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "hoho = 10;"
    "hehe = haha * 3;"
    "if (1 < more) then {"
    "huhu = hihi * 5;"
    "} else {"
    "hbhb = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    THEN("The processing should fail") {
      REQUIRE_THROWS(SPFacade.populatePKB());
    }
  }
}

TEST_CASE("Integration test SP to PKB with excessive extraneous brackets surrounding cond_expr") {
  std::string testProgram = "procedure test {"
    "if ((((((less < b + c * d)))))) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "hoho = 10;"
    "hehe = haha * 3;"
    "if (1 < more) then {"
    "huhu = hihi * 5;"
    "} else {"
    "hbhb = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    THEN("The processing should fail") {
      REQUIRE_THROWS(SPFacade.populatePKB());
    }
  }
}

TEST_CASE("Integration test SP to PKB with wrongly used negation and brackets surrounding cond_expr") {
  std::string testProgram = "procedure test {"
    "if ((a < b) || !(5 < 15 || 5 > 15)) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "hoho = 10;"
    "hehe = haha * 3;"
    "if (1 < more) then {"
    "huhu = hihi * 5;"
    "} else {"
    "hbhb = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    THEN("The processing should fail") {
      REQUIRE_THROWS(SPFacade.populatePKB());
    }
  }
}

TEST_CASE("Integration test SP to PKB with valid extraneous negations surrounding cond_expr") {
  std::string testProgram = "procedure test {"
    "if (!(!(!(!(i < like + testing * exclamation))))) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "hoho = 10;"
    "hehe = haha * 3;"
    "if (1 < more) then {"
    "huhu = hihi * 5;"
    "} else {"
    "hbhb = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    THEN("The processing should not throw any exception") {
      REQUIRE_NOTHROW(SPFacade.populatePKB());
    }
  }
}
