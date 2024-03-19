#include "../lib/catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("If & While Pattern Evaluator") {
  SECTION("Evaluate entity evaluator with IDENT as left argument") {
	SECTION("WILDCARD right argument non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: IDENT, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query if ifs; Select if such that pattern if(TEST_IDENT, _, _);" << std::endl;
	  std::cout << "Testing query while w; Select w such that pattern w(TEST_IDENT, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertIf(2);
	  writer->insertIf(3);
	  writer->insertIfPattern("testVar1", 2);

	  writer->insertWhile(4);
	  writer->insertWhile(5);
	  writer->insertWhilePattern("testVar2", 5);

	  auto wildcardSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  auto lArgIfSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");

	  PQL::Clause ifPatternClause =
		  PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF, {ifSyn, lArgIfSyn, wildcardSyn, wildcardSyn});
	  PQL::Query q = PQL::Query({ifSyn}, {ifPatternClause}, ifSyn);
	  std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedIfRes = {"2"};
	  REQUIRE(res == expectedIfRes);

	  auto whileSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  auto lArgWhileSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar2");
	  PQL::Clause whilePatternClause =
		  PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE, {whileSyn, lArgWhileSyn, wildcardSyn});
	  q = PQL::Query({whileSyn}, {whilePatternClause}, whileSyn);
	  res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedWhileRes = {"5"};
	  REQUIRE(res == expectedWhileRes);
	}

	SECTION("WILDCARD right argument empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: IDENT, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query if ifs; Select if such that pattern if(TEST_IDENT, _, _);" << std::endl;
	  std::cout << "Testing query while w; Select w such that pattern w(TEST_IDENT, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertIf(2);
	  writer->insertIf(3);
	  writer->insertIfPattern("testVar1", 2);

	  writer->insertWhile(4);
	  writer->insertWhile(5);
	  writer->insertWhilePattern("testVar2", 5);

	  auto wildcardSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  auto lArgIfSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar2");

	  PQL::Clause ifPatternClause =
		  PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF, {ifSyn, lArgIfSyn, wildcardSyn, wildcardSyn});
	  PQL::Query q = PQL::Query({ifSyn}, {ifPatternClause}, ifSyn);
	  std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);

	  auto whileSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  auto lArgWhileSyn = PQL::Synonym(SimpleProgram::DesignEntity::IDENT, "testVar1");
	  PQL::Clause whilePatternClause =
		  PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE, {whileSyn, lArgWhileSyn, wildcardSyn});
	  q = PQL::Query({whileSyn}, {whilePatternClause}, whileSyn);
	  res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  REQUIRE(res == expectedRes);
	}
  }

  SECTION("Evaluate entity evaluator with VAR as left argument") {
	SECTION("WILDCARD right argument non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: VAR, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query variable v; if ifs; Select if such that pattern if(v, _, _);" << std::endl;
	  std::cout << "Testing query variable v; while w; Select w such that pattern w(v, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertIf(2);
	  writer->insertIf(3);
	  writer->insertIfPattern("testVar1", 2);

	  writer->insertWhile(4);
	  writer->insertWhile(5);
	  writer->insertWhilePattern("testVar2", 5);

	  auto wildcardSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");

	  PQL::Clause ifPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF,
												{ifSyn, varSyn, wildcardSyn, wildcardSyn});
	  PQL::Query q = PQL::Query({varSyn, ifSyn}, {ifPatternClause}, ifSyn);
	  std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedIfRes = {"2"};
	  REQUIRE(res == expectedIfRes);

	  auto whileSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  PQL::Clause whilePatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE,
												   {whileSyn, varSyn, wildcardSyn});
	  q = PQL::Query({varSyn, whileSyn}, {whilePatternClause}, whileSyn);
	  res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedWhileRes = {"5"};
	  REQUIRE(res == expectedWhileRes);
	}

	SECTION("WILDCARD right argument non-empty and correct variable") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: VAR, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query variable v; if ifs; Select v such that pattern if(v, _, _);" << std::endl;
	  std::cout << "Testing query variable v; while w; Select v such that pattern w(v, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertIf(2);
	  writer->insertIf(3);
	  writer->insertIfPattern("testVar1", 2);

	  writer->insertWhile(4);
	  writer->insertWhile(5);
	  writer->insertWhilePattern("testVar2", 5);

	  auto wildcardSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");

	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  PQL::Clause ifPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF,
												{ifSyn, varSyn, wildcardSyn, wildcardSyn});
	  PQL::Query q = PQL::Query({varSyn, ifSyn}, {ifPatternClause}, varSyn);
	  std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedIfRes = {"testVar1"};
	  REQUIRE(res == expectedIfRes);

	  auto whileSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  PQL::Clause whilePatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE,
												   {whileSyn, varSyn, wildcardSyn});
	  q = PQL::Query({varSyn, whileSyn}, {whilePatternClause}, varSyn);
	  res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedWhileRes = {"testVar2"};
	  REQUIRE(res == expectedWhileRes);
	}

	SECTION("WILDCARD right argument empty for variable") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query variable v; if ifs; Select v such that pattern if(v, _, _);" << std::endl;
	  std::cout << "Testing query variable v; while w; Select v such that pattern w(v, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertIf(2);
	  writer->insertIf(3);

	  writer->insertWhile(4);
	  writer->insertWhile(5);

	  auto wildcardSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<std::string> expectedRes = {};

	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  PQL::Clause ifPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF,
												{ifSyn, varSyn, wildcardSyn, wildcardSyn});
	  PQL::Query q = PQL::Query({varSyn, ifSyn}, {ifPatternClause}, varSyn);
	  std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  REQUIRE(res == expectedRes);

	  auto whileDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  PQL::Clause whilePatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE,
												   {whileDeclaration, varSyn, wildcardSyn});
	  q = PQL::Query({varSyn, whileDeclaration}, {whilePatternClause}, varSyn);
	  res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedWhileRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("WILDCARD right argument empty for statement") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: VAR, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query variable v; if ifs; Select ifs such that pattern if(v, _, _);" << std::endl;
	  std::cout << "Testing query variable v; while w; Select w such that pattern w(v, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertIf(2);
	  writer->insertIf(3);

	  writer->insertWhile(4);
	  writer->insertWhile(5);

	  auto wildcardSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<std::string> expectedRes = {};

	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  PQL::Clause ifPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF,
												{ifSyn, varSyn, wildcardSyn, wildcardSyn});
	  PQL::Query q = PQL::Query({varSyn, ifSyn}, {ifPatternClause}, ifSyn);
	  std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  REQUIRE(res == expectedRes);

	  auto whileSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  PQL::Clause whilePatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE,
												   {whileSyn, varSyn, wildcardSyn});
	  q = PQL::Query({varSyn, whileSyn}, {whilePatternClause}, whileSyn);
	  res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  std::vector<std::string> expectedWhileRes = {};
	  REQUIRE(res == expectedRes);
	}
  }

  SECTION("Evaluate entity evaluator with WILDCARD as left argument") {
	SECTION("WILDCARD right argument non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: WILDCARD, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query variable v; if ifs; Select v such that pattern if(_, _, _);" << std::endl;
	  std::cout << "Testing query variable v; while w; Select v such that pattern w(_, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertVariable("testVar3");
	  writer->insertIf(2);
	  writer->insertIf(3);
	  writer->insertIfPattern("testVar1", 2);

	  writer->insertWhile(4);
	  writer->insertWhile(5);
	  writer->insertWhilePattern("testVar2", 5);

	  auto wildcardSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<std::string> expectedRes = {"testVar1", "testVar2", "testVar3"};

	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  PQL::Clause ifPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF,
												{ifSyn, wildcardSyn, wildcardSyn, wildcardSyn});
	  PQL::Query q = PQL::Query({varSyn, ifSyn}, {ifPatternClause}, selectSyn);
	  std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  REQUIRE(checkVecValuesEqual(res, expectedRes));

	  auto whileSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  PQL::Clause whilePatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE,
												   {whileSyn, wildcardSyn, wildcardSyn});
	  q = PQL::Query({varSyn, whileSyn}, {whilePatternClause}, selectSyn);
	  res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}

	SECTION("WILDCARD right argument empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: WILDCARD, rArg: WILDCARD" << std::endl;
	  std::cout << "Testing query variable v; if ifs; Select v such that pattern if(_, _, _);" << std::endl;
	  std::cout << "Testing query variable v; while w; Select v such that pattern w(_, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertVariable("testVar2");
	  writer->insertVariable("testVar3");
	  writer->insertIf(2);
	  writer->insertIf(3);

	  writer->insertWhile(4);
	  writer->insertWhile(5);

	  auto wildcardSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto varSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<std::string> expectedRes = {};

	  auto ifSyn = PQL::Synonym(SimpleProgram::DesignEntity::IF, "ifs");
	  PQL::Clause ifPatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_IF,
												{ifSyn, wildcardSyn, wildcardSyn, wildcardSyn});
	  PQL::Query q = PQL::Query({varSyn, ifSyn}, {ifPatternClause}, selectSyn);
	  std::vector<std::string> res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  REQUIRE(checkVecValuesEqual(res, expectedRes));

	  auto whileSyn = PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "w");
	  PQL::Clause whilePatternClause = PQL::Clause(SimpleProgram::DesignAbstraction::PATTERN_WHILE,
												   {whileSyn, wildcardSyn, wildcardSyn});
	  q = PQL::Query({varSyn, whileSyn}, {whilePatternClause}, selectSyn);
	  res = QueryEvaluator::QueryEvaluator(reader).evaluateQuery(q);
	  REQUIRE(checkVecValuesEqual(res, expectedRes));
	}
  }
}