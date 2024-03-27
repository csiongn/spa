#include "../lib/catch.hpp"
#include "QPS/QueryTokenizer.h"
#include "QPS/QuerySyntaxError.h"
#include "Models/PQL.h"
#include "QPS/Parser/PatternClauseParser.h"

TEST_CASE("Pattern Clause Parser") {
  QueryTokenizer tokenizer{};
  std::vector<PQL::Synonym> declarations{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a"),
										 PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs"),
										 PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w")};

  std::string patternQ1 = R"(pattern a(_, _))";
  std::string patternQ2 = R"(pattern a("a", _))";
  std::string patternQ3 = R"(pattern a(_, "x+y"))";
  std::string patternQ4 = R"(pattern a(_, _"x+y"_))";
  std::string patternQ5 = R"(pattern a("a", "x+y"))";

  SECTION("Parse pattern assign query") {
	SECTION("LArg wildcard, RArg wildcard") {
	  auto q1Tokens = std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(tokenizer.tokenize(patternQ1));
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(declarations));
	  PatternClauseParser parser(q1Tokens, validator, declarations);
	  PQL::Clause patternClause = parser.parse(*q1Tokens);

	  REQUIRE_NOTHROW(patternClause);

	  PQL::Clause expectedClause{SimpleProgram::DesignAbstraction::PATTERN_ASSIGN,
								 std::vector<PQL::Synonym>{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a"),
														   PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_"),
														   PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_")}};
	  REQUIRE(patternClause == expectedClause);
	}

	SECTION("LArg IDENT, RArg wildcard") {
	  auto q2Tokens = std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(tokenizer.tokenize(patternQ2));
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(declarations));
	  PatternClauseParser parser(q2Tokens, validator, declarations);
	  PQL::Clause patternClause = parser.parse(*q2Tokens);

	  REQUIRE_NOTHROW(patternClause);

	  PQL::Clause expectedClause{SimpleProgram::DesignAbstraction::PATTERN_ASSIGN,
								 std::vector<PQL::Synonym>{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a"),
														   PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "a"),
														   PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_")}};
	  REQUIRE(patternClause == expectedClause);
	}

	SECTION("LArg wildcard, RArg EXPRESSION") {
	  auto q3Tokens = std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(tokenizer.tokenize(patternQ3));
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(declarations));
	  PatternClauseParser parser(q3Tokens, validator, declarations);
	  PQL::Clause patternClause = parser.parse(*q3Tokens);

	  REQUIRE_NOTHROW(patternClause);

	  PQL::Clause expectedClause{SimpleProgram::DesignAbstraction::PATTERN_ASSIGN,
								 std::vector<PQL::Synonym>{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a"),
														   PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_"),
														   PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "x+y")}};
	  REQUIRE(patternClause == expectedClause);
	}

	SECTION("LArg wildcard, RArg PARTIAL_EXPRESSION") {
	  auto q4Tokens = std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(tokenizer.tokenize(patternQ4));
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(declarations));
	  PatternClauseParser parser(q4Tokens, validator, declarations);
	  PQL::Clause patternClause = parser.parse(*q4Tokens);

	  REQUIRE_NOTHROW(patternClause);

	  PQL::Clause expectedClause{SimpleProgram::DesignAbstraction::PATTERN_ASSIGN,
								 std::vector<PQL::Synonym>{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a"),
														   PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_"),
														   PQL::Synonym(SimpleProgram::DesignEntity::PARTIAL_EXPR,
																		"_x+y_")}};
	  REQUIRE(patternClause == expectedClause);
	}

	SECTION("LArg IDENT, RArg EXPRESSION") {
	  auto q5Tokens = std::make_shared<std::vector<std::shared_ptr<QueryToken>>>(tokenizer.tokenize(patternQ5));
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(declarations));
	  PatternClauseParser parser(q5Tokens, validator, declarations);
	  PQL::Clause patternClause = parser.parse(*q5Tokens);

	  REQUIRE_NOTHROW(patternClause);

	  PQL::Clause expectedClause{SimpleProgram::DesignAbstraction::PATTERN_ASSIGN,
								 std::vector<PQL::Synonym>{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a"),
														   PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "a"),
														   PQL::Synonym(SimpleProgram::DesignEntity::EXPR, "x+y")}};
	  REQUIRE(patternClause == expectedClause);
	}
  }
}