#include "QPS/QueryTokenizer.h"
#include "../TestUtils.h"
#include "../lib/catch.hpp"
#include "QPS/QuerySyntaxError.h"

bool checkTokenVectorEqual(std::vector<std::shared_ptr<QueryToken>> vec1, std::vector<QueryToken> vec2) {
  if (vec1.size() != vec2.size()) {
	return false;
  }
  for (int i = 0; i < vec1.size(); i++) {
	REQUIRE(vec1[i]->getTypeString() == vec2[i].getTypeString());
	REQUIRE(vec1[i]->getValue() == vec2[i].getValue());
  }
  for (int i = 0; i < vec1.size(); i++) {
	if (vec1[i]->getTypeString() != vec2[i].getTypeString() || vec1[i]->getValue() != vec2[i].getValue()) {
	  return false;
	}
  }
  return true;
}

void printTokens(std::vector<std::shared_ptr<QueryToken>> tokens) {
  for (const auto &token : tokens) {
	std::cout << token->getTypeString() << " " << token->getValue() << std::endl;
  }
}
TEST_CASE("Tokenizer") {
  SECTION("TOKENIZE") {
	QueryTokenizer queryTokenizer;
	std::string query =
		"assign a1, a2; variable v; \nSelect s12d such that Follows*(1,\"sasds\") pattern a(_, _\" (x+y)+1   \"_)";

	auto tokens = queryTokenizer.tokenize(query);
	// Compare tokens
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "assign"),
		QueryToken(QPS::TokenType::NAME, "a1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::NAME, "a2"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "variable"),
		QueryToken(QPS::TokenType::NAME, "v"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "s12d"),
		QueryToken(QPS::TokenType::NAME, "such"),
		QueryToken(QPS::TokenType::NAME, "that"),
		QueryToken(QPS::TokenType::NAME, "Follows*"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::INTEGER, "1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::CONSTANT_STRING, "sasds"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")"),
		QueryToken(QPS::TokenType::NAME, "pattern"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::EXPRESSION, "(x+y)+1"),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")")
	};

	// check each vector has to had the same typestring and value
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Valid Tokens") {
	QueryTokenizer queryTokenizer;
	std::string query = "Select s12d such that Follows* (s123, \"x*y\")";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectTokensVector = {
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "s12d"),
		QueryToken(QPS::TokenType::NAME, "such"),
		QueryToken(QPS::TokenType::NAME, "that"),
		QueryToken(QPS::TokenType::NAME, "Follows*"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::NAME, "s123"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::EXPRESSION, "x*y"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")")
	};
	// check each vector has to had the same typestring and value
	REQUIRE(checkTokenVectorEqual(tokens, expectTokensVector));
  }

  SECTION("Invalid Tokens in MS1 and Synonym used more than once") {
	QueryTokenizer queryTokenizer;
	std::string query =
		"procedure p, q;\nSelect BOOLEAN such that Calls(p, q) with q.procName = \"p\" and p.procName = \"Example\"";
	// Tokens invalid in MS1, Tokens now valid in MS2
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "procedure"),
		QueryToken(QPS::TokenType::NAME, "p"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::NAME, "q"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "BOOLEAN"),
		QueryToken(QPS::TokenType::NAME, "such"),
		QueryToken(QPS::TokenType::NAME, "that"),
		QueryToken(QPS::TokenType::NAME, "Calls"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::NAME, "p"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::NAME, "q"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")"),
		QueryToken(QPS::TokenType::NAME, "with"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "q.procName"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::CONSTANT_STRING, "p"),
		QueryToken(QPS::TokenType::NAME, "and"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "p.procName"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::CONSTANT_STRING, "Example")
	};
  }

  SECTION("Valid Tokens with whitespace inside constant string") {
	QueryTokenizer queryTokenizer;
	std::string query = "assign a;\n Select a pattern a (_, _\"1 \"_)";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "assign"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::NAME, "pattern"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::CONSTANT_STRING, "1"),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Valid Tokens with whitespace inside constant string") {
	QueryTokenizer queryTokenizer;
	std::string query = "assign a;\n Select a pattern a (_, _\"quota \t\"_)";
	auto tokens = queryTokenizer.tokenize(query);

	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "assign"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::NAME, "pattern"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::CONSTANT_STRING, "quota"),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Valid Tokens with whitespace inside constant string") {
	QueryTokenizer queryTokenizer;
	std::string query = "assign a;\n Select a pattern a (_, _\"x * 2 \t\"_)";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "assign"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::NAME, "pattern"),
		QueryToken(QPS::TokenType::NAME, "a"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::EXPRESSION, "x*2"),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Valid Tokens with whitespace after Follows*") {
	QueryTokenizer queryTokenizer;
	std::string query = "stmt s12d;\n Select s12d such that Follows* (s, \"x*y\")";
	REQUIRE_NOTHROW(queryTokenizer.tokenize(query));
  }

  SECTION("Invalid Tokens with whitespace between Follows and *") {
	QueryTokenizer queryTokenizer;
	std::string query = "assign a; variable v; constant c; Select a such that Follows * (a, c) pattern a (\"8\", _)";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Invalid NAME Tokens - Should throw QuerySyntaxError") {
	QueryTokenizer queryTokenizer;
	std::string query = "assign a&&; constant c; Select a such that Uses (a, c)";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Helper function - Process Apostrophe") {
	QueryTokenizer queryTokenizer;
	std::string query = "\"   x +     quota     % 5 - 10 / a\"";
	queryTokenizer.tokenize(query);
  }

  SECTION("Helper function - Process Apostrophe") {
	QueryTokenizer queryTokenizer;
	std::string query = "\"he llo      + 1\"";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Helper function - Process Apostrophe - Pass CONSTANT_STRING single token") {
	QueryTokenizer queryTokenizer;
	std::string query = "\" 10      \"";
	auto tokenResults = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::CONSTANT_STRING, "10")
	};
	REQUIRE(checkTokenVectorEqual(tokenResults, expectedTokensVector));
  }

  SECTION("Helper function - Process Apostrophe - Pass CONSTANT_STRING single token") {
	QueryTokenizer queryTokenizer;
	std::string query = "\"    hello       \"";
	auto tokenResults = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::CONSTANT_STRING, "hello")
	};
	REQUIRE(checkTokenVectorEqual(tokenResults, expectedTokensVector));
  }

  SECTION("Helper function - Process Apostrophe - Invalid Special Character '&' QuerySyntaxError") {
	QueryTokenizer queryTokenizer;
	std::string query = "\"   x +     quota     % 5 - 10 && a\"";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Invalid NAME token - SyntaxError") {
	QueryTokenizer queryTokenizer;
	std::string query = "Select 1v";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tokenize Attribute value") {
	QueryTokenizer queryTokenizer;
	std::string query = "stmt.stmt# read.stmt# print.stmt# call.stmt# while.stmt# if.stmt# assign.stmt#";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "stmt.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "read.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "print.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "call.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "while.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "if.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "assign.stmt#")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Tokenize Attribute name") {
	QueryTokenizer queryTokenizer;
	std::string query = "procedure.procName call.procName variable.varName read.varName print.varName";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "procedure.procName"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "call.procName"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "variable.varName"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "read.varName"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "print.varName")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Tokenize mixed attribute name and value for call and read") {
	QueryTokenizer queryTokenizer;
	std::string query = "call.procName read.stmt#";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "call.procName"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "read.stmt#")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Tokenize mixed attribute name, value, constant") {
	QueryTokenizer queryTokenizer;
	std::string query =
		"stmt.stmt# read.stmt# print.stmt# call.procName print.varName read.varName assign.stmt# constant.value";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "stmt.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "read.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "print.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "call.procName"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "print.varName"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "read.varName"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "assign.stmt#"),
		QueryToken(QPS::TokenType::ATTRIBUTE_CONSTANT, "constant.value")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Tokenize query with attribute") {
	QueryTokenizer queryTokenizer;
	std::string query = "stmt s; constant c;\n Select s with s.stmt# = c.value";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "stmt"),
		QueryToken(QPS::TokenType::NAME, "s"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "constant"),
		QueryToken(QPS::TokenType::NAME, "c"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "s"),
		QueryToken(QPS::TokenType::NAME, "with"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "s.stmt#"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::ATTRIBUTE_CONSTANT, "c.value")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Invalid Attribute field") {
	QueryTokenizer queryTokenizer;
	std::string query = "stmt s; constant c;\n Select s with s.line# = c.constant";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tuple string") {
	QueryTokenizer queryTokenizer;
	std::string query = "< x , s.stmt# >";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::TUPLE, "<x,s.stmt#>"),
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Tuple + Attribute Query") {
	QueryTokenizer queryTokenizer;
	std::string query = "Select < x , s.stmt# > with s.stmt# = 1";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::TUPLE, "<x,s.stmt#>"),
		QueryToken(QPS::TokenType::NAME, "with"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "s.stmt#"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::INTEGER, "1")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  // Test whitespace before attributes both before full stop and after
  SECTION("Attribute with whitespace - 1") {
	QueryTokenizer queryTokenizer;
	std::string query = "stmt s; Select s with s   .stmt#=1";
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "stmt"),
		QueryToken(QPS::TokenType::NAME, "s"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "s"),
		QueryToken(QPS::TokenType::NAME, "with"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "s.stmt#"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::INTEGER, "1")
	};
	REQUIRE(checkTokenVectorEqual(queryTokenizer.tokenize(query), expectedTokensVector));
  }
  SECTION("Attribute with whitespace - 2") {
	QueryTokenizer queryTokenizer;
	std::string query = "stmt s; Select s with s    .      stmt#=1";
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "stmt"),
		QueryToken(QPS::TokenType::NAME, "s"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "s"),
		QueryToken(QPS::TokenType::NAME, "with"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "s.stmt#"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::INTEGER, "1")
	};
	REQUIRE(checkTokenVectorEqual(queryTokenizer.tokenize(query), expectedTokensVector));
  }

  SECTION("Attribute with whitespace - 3") {
	QueryTokenizer queryTokenizer;
	std::string query = "stmt s; Select s with s.stmt#=1";
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "stmt"),
		QueryToken(QPS::TokenType::NAME, "s"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::NAME, "s"),
		QueryToken(QPS::TokenType::NAME, "with"),
		QueryToken(QPS::TokenType::ATTRIBUTE_VALUE, "s.stmt#"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::INTEGER, "1")
	};
	REQUIRE(checkTokenVectorEqual(queryTokenizer.tokenize(query), expectedTokensVector));
  }

  SECTION("Valid Tuple with whitespace") {
	QueryTokenizer queryTokenizer;
	std::string query = " <w1, p1>";
	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::TUPLE, "<w1,p1>"),
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Multi clause") {
	QueryTokenizer queryTokenizer;
	std::string query =
		"while w1; procedure p1; print p1; read r1; variable v1;"\
        "Select <w1, p1> such that Next(_, pn1) with r1.varName   =        p1.procName with \"a\"=      v1.varName pattern w1(_, _)";

	auto tokens = queryTokenizer.tokenize(query);
	std::vector<QueryToken> expectedTokensVector = {
		QueryToken(QPS::TokenType::NAME, "while"),
		QueryToken(QPS::TokenType::NAME, "w1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "procedure"),
		QueryToken(QPS::TokenType::NAME, "p1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "print"),
		QueryToken(QPS::TokenType::NAME, "p1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "read"),
		QueryToken(QPS::TokenType::NAME, "r1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "variable"),
		QueryToken(QPS::TokenType::NAME, "v1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ";"),
		QueryToken(QPS::TokenType::NAME, "Select"),
		QueryToken(QPS::TokenType::TUPLE, "<w1,p1>"),
		QueryToken(QPS::TokenType::NAME, "such"),
		QueryToken(QPS::TokenType::NAME, "that"),
		QueryToken(QPS::TokenType::NAME, "Next"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::NAME, "pn1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")"),
		QueryToken(QPS::TokenType::NAME, "with"),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "r1.varName"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "p1.procName"),
		QueryToken(QPS::TokenType::NAME, "with"),
		QueryToken(QPS::TokenType::CONSTANT_STRING, "a"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "="),
		QueryToken(QPS::TokenType::ATTRIBUTE_NAME, "v1.varName"),
		QueryToken(QPS::TokenType::NAME, "pattern"),
		QueryToken(QPS::TokenType::NAME, "w1"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, "("),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ","),
		QueryToken(QPS::TokenType::WILDCARD, "_"),
		QueryToken(QPS::TokenType::SPECIAL_CHARACTER, ")")
	};
	REQUIRE(checkTokenVectorEqual(tokens, expectedTokensVector));
  }

  SECTION("Multi clause 1") {
	QueryTokenizer queryTokenizer;
	std::string query =
		"if ifs; while w;"
		"Select BOOLEAN pattern w (\"x\", _) and ifs (\"x\", _, _) such that Parent(w, ifs)";

	auto tokens = queryTokenizer.tokenize(query);
  }

  SECTION("Tuple") {
	QueryTokenizer queryTokenizer;
	std::string query = "assign a; while w; constant c;\n"
						"Select <a.stmt#, w.varName,    b.stmt#> ";
	auto tokens = queryTokenizer.tokenize(query);
  }

  SECTION("Tuple Syntax Error - Comma at the end") {
	QueryTokenizer queryTokenizer;
	std::string query = "<a , w   , s  , ss,>";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tuple Syntax Error - Comma at the front") {
	QueryTokenizer queryTokenizer;
	std::string query = "<,a , w,y,z, g>";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tuple Syntax Error - Missing comma") {
	QueryTokenizer queryTokenizer;
	std::string query = "<s s1, s2>";

	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tuple Syntax Error - Extra comma") {
	QueryTokenizer queryTokenizer;
	std::string query = "stmt s, s1; Select <s, , s1>";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tuple Valid - No whitespace") {
	QueryTokenizer queryTokenizer;
	std::string query = "<s,s1,s2,ss,p1,p.varName,a.stmt#>";
	queryTokenizer.tokenize(query);
	REQUIRE_NOTHROW(queryTokenizer.tokenize(query));
  }

  SECTION("Tuple Valid - Valid Attribute Fields") {
	QueryTokenizer queryTokenizer;
	std::string query = "<s.stmt#,s1.varName,s2.procName,c.value>";
	REQUIRE_NOTHROW(queryTokenizer.tokenize(query));
  }

  SECTION("Tuple Syntax Error - Invalid stmt# Attribute Fields") {
	QueryTokenizer queryTokenizer;
	std::string query = "<s.stmt,s1.varName,s2.procName,c.value>";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tuple Syntax Error - Invalid varName Attribute Fields") {
	QueryTokenizer queryTokenizer;
	std::string query = "<s.stmt#,s1.variableName,s2.procName,c.value>";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tuple Syntax Error - Invalid procName Attribute Fields") {
	QueryTokenizer queryTokenizer;
	std::string query = "<s.stmt#,s1.varName,s2.procedureName,c.value>";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }

  SECTION("Tuple Syntax Error - Invalid value Attribute Fields") {
	QueryTokenizer queryTokenizer;
	std::string query = "<s.stmt#,s1.varName,s2.procName,c.value12>";
	REQUIRE_THROWS_AS(queryTokenizer.tokenize(query), QuerySyntaxError);
  }


}
