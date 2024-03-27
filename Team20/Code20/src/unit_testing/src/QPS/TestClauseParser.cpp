#include "../lib/catch.hpp"
#include "QPS/Parser/ClauseParser.h"
#include "QPS/QueryTokenizer.h"
#include "QPS/QuerySyntaxError.h"

TEST_CASE("Clause Parser") {
  QueryTokenizer tokenizer{};
  std::string selectQ1 = R"(Select a)";
  std::vector<PQL::Synonym> selectQ1Declarations{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a"),
												 PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c")};
  std::string selectQ2 = R"(Select <a1,c1>)";
  std::vector<PQL::Synonym> selectQ2Declarations{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a1"),
												 PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a2"),
												 PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c1"),
												 PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c2")};

  SECTION("Parse select query") {
	SECTION("Single value") {
	  std::vector<std::shared_ptr<QueryToken>> selectQ1Tokens = tokenizer.tokenize(selectQ1);
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(selectQ1Declarations));
	  ClauseParser parser(selectQ1Tokens, selectQ1Declarations, validator);
	  std::vector<PQL::Synonym> selectSynonyms = parser.parseSelectClause();

	  REQUIRE_NOTHROW(selectSynonyms);

	  std::vector<PQL::Synonym> expectedSelectSynonyms{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a")};
	  REQUIRE(selectSynonyms == expectedSelectSynonyms);
	}

	SECTION("Multiple values (tuple)") {
	  std::vector<std::shared_ptr<QueryToken>> selectQ2Tokens = tokenizer.tokenize(selectQ2);
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(selectQ2Declarations));
	  ClauseParser parser(selectQ2Tokens, selectQ2Declarations, validator);
	  std::vector<PQL::Synonym> selectSynonyms = parser.parseSelectClause();

	  REQUIRE_NOTHROW(selectSynonyms);

	  std::vector<PQL::Synonym> expectedSelectSynonyms{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "a1"),
													   PQL::Synonym(SimpleProgram::DesignEntity::CONSTANT, "c1")};
	  REQUIRE(selectSynonyms == expectedSelectSynonyms);
	}

	SECTION("Invalid select clause") {
	  std::string invalidSelectQ = R"(Select)";
	  std::vector<std::shared_ptr<QueryToken>> invalidSelectQTokens = tokenizer.tokenize(invalidSelectQ);
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(selectQ1Declarations));
	  ClauseParser parser(invalidSelectQTokens, selectQ1Declarations, validator);

	  REQUIRE_THROWS_AS(parser.parseSelectClause(), QuerySyntaxError);
	}

	SECTION("Invalid select clause \"select\"") {
	  std::string invalidSelectQ = R"(select a)";
	  std::vector<std::shared_ptr<QueryToken>> invalidSelectQTokens = tokenizer.tokenize(invalidSelectQ);
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(selectQ1Declarations));
	  ClauseParser parser(invalidSelectQTokens, selectQ1Declarations, validator);

	  REQUIRE_THROWS_AS(parser.parseSelectClause(), QuerySyntaxError);
	}

	SECTION("\"Select\" as assign synonym clause") {
	  std::string selectQ = R"(Select Select)";
	  std::vector<PQL::Synonym> selectQDeclarations{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "Select")};
	  std::vector<std::shared_ptr<QueryToken>> invalidSelectQTokens = tokenizer.tokenize(selectQ);
	  auto validator = std::make_shared<ClauseValidator>(ClauseValidator(selectQDeclarations));
	  ClauseParser parser(invalidSelectQTokens, selectQDeclarations, validator);
	  std::vector<PQL::Synonym> selectSynonyms = parser.parseSelectClause();

	  REQUIRE_NOTHROW(selectSynonyms);

	  std::vector<PQL::Synonym> expectedSelectSynonyms{PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "Select")};
	  REQUIRE(selectSynonyms == expectedSelectSynonyms);
	}
  }

  SECTION("Parse relationship clauses") {
  }
}