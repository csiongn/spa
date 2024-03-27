#include "../lib/catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("Statement Evaluator") {
  SECTION("Evaluate statement only queries with two statement number arguments") {
	SECTION("follows and non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with follows that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Follow(2,3);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(2, 3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
	  auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {followClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testVar1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("follows and empty (same statement number)") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with follows that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Follow(2,2);" << std::endl;

	  writer->insertVariable("testVar1");
	  // not supposed to have such relationship, added just to test
	  writer->insertFollowsT(2, 2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {followClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("follows and empty (different statement number)") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with follows that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Follow(2,3);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
	  auto followClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {followClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("followsT and non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with follows* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Follow*(2,5);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollowsT(2, 5);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "5");
	  auto followTClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {followTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testVar1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("followsT and empty (same statement number)") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with follows* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Follow*(2,2);" << std::endl;

	  writer->insertVariable("testVar1");
	  // not supposed to have such relationship, added just to test
	  writer->insertFollowsT(2, 2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto followTClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {followTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("followsT and empty (different statement number)") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with follows* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Follow*(2,3);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
	  auto followTClause = PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {followTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("parent and non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Parent(2,3);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertParent(2, 3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
	  auto parentClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testVar1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("parent and empty (same statement number") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Parent(2,2);" << std::endl;

	  writer->insertVariable("testVar1");
	  // not supposed to have such relationship, added just to test
	  writer->insertFollowsT(2, 2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto parentClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("parent and empty (different statement number") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Parent(2,3);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
	  auto parentClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("parentT and non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Parent*(2,5);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertParentT(2, 5);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "5");
	  auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testVar1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("parentT and empty (same statement number)") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Parent*(2,2);" << std::endl;

	  writer->insertVariable("testVar1");
	  // not supposed to have such relationship, added just to test
	  writer->insertFollowsT(2, 2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("parentT and empty (different statement number)") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Parent*(2,3);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
	  auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("Next and non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Next(2,5);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertNext(2, 5);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "5");
	  auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {"testVar1"};
	  REQUIRE(res == expectedRes);
	}

	SECTION("Next and empty (same statement number)") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Parent*(2,2);" << std::endl;

	  writer->insertVariable("testVar1");
	  // not supposed to have such relationship, added just to test
	  writer->insertNext(2, 2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}

	SECTION("Next and empty (different statement number)") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with parent* that has both statement number as arguments." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that Next(2,3);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "3");
	  auto parentTClause = PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn});
	  auto q = PQL::Query({varDeclaration}, {parentTClause}, {selectSyn});

	  auto res = evaluator.evaluateQuery(q);
	  std::vector<std::string> expectedRes = {};
	  REQUIRE(res == expectedRes);
	}
  }

  SECTION("Evaluate statement only queries with STMT_NUM as left argument") {
	SECTION("WILDCARD non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: STMT_NUM, rArg: WILDCARD."
				<< std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(2, 3);
	  writer->insertFollowsT(2, 3);
	  writer->insertParent(2, 3);
	  writer->insertParentT(2, 3);
	  writer->insertNext(2, 3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn})
	  };

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &cl : clauses) {
		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::FOLLOWS:
			std::cout << "FOLLOWS passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::FOLLOWST:
			std::cout << "FOLLOWST passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENT:
			std::cout << "PARENT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENTT:
			std::cout << "PARENTT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::NEXT:
			std::cout << "TESTING NEXT" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}
	  }
	}

	SECTION("WILDCARD empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: WILDCARD." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, _);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn})
	  };

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &cl : clauses) {
		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::FOLLOWS:
			std::cout << "FOLLOWS passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::FOLLOWST:
			std::cout << "FOLLOWST passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENT:
			std::cout << "PARENT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENTT:
			std::cout << "PARENTT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::NEXT:
			std::cout << "TESTING NEXT" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: STMT_NUM, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN such that XXX(2, SYN);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(2, 3);
	  writer->insertFollowsT(2, 3);
	  writer->insertParent(2, 3);
	  writer->insertParentT(2, 3);
	  writer->insertNext(2, 3);
	  writer->insertStatement(3);
	  writer->insertRead(3);
	  writer->insertPrint(3);
	  writer->insertAssign(3);
	  writer->insertCall(3);
	  writer->insertWhile(3);
	  writer->insertIf(3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, lArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {lArgSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"3"};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, syn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: STMT_NUM, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, SYN);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(2, 3);
	  writer->insertFollowsT(2, 3);
	  writer->insertParent(2, 3);
	  writer->insertParentT(2, 3);
	  writer->insertNext(2, 3);
	  writer->insertStatement(3);
	  writer->insertRead(3);
	  writer->insertPrint(3);
	  writer->insertAssign(3);
	  writer->insertCall(3);
	  writer->insertWhile(3);
	  writer->insertIf(3);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, lArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, syn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: STMT_NUM, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(2, SYN);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, lArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, syn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}
  }

  SECTION("Evaluate statement only queries with STMT_NUM as right argument") {
	SECTION("WILDCARD non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: WILDCARD, rArg: STMT_NUM."
				<< std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(_, 2);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn})
	  };

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &cl : clauses) {
		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::FOLLOWS:
			std::cout << "FOLLOWS passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::FOLLOWST:
			std::cout << "FOLLOWST passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENT:
			std::cout << "PARENT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENTT:
			std::cout << "PARENTT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::NEXT:
			std::cout << "TESTING NEXT" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}
	  }
	}

	SECTION("WILDCARD empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: STMT_NUM." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(_, 2);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn})
	  };

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &cl : clauses) {
		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::FOLLOWS:
			std::cout << "FOLLOWS passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::FOLLOWST:
			std::cout << "FOLLOWST passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENT:
			std::cout << "PARENT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENTT:
			std::cout << "PARENTT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::NEXT:
			std::cout << "TESTING NEXT" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: STMT_NUM." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN such that XXX(SYN, 2);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);
	  writer->insertStatement(1);
	  writer->insertRead(1);
	  writer->insertPrint(1);
	  writer->insertAssign(1);
	  writer->insertCall(1);
	  writer->insertWhile(1);
	  writer->insertIf(1);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {rArgSyn, rArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {rArgSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"1"};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: STMT_NUM." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(SYN, 2);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);
	  writer->insertStatement(1);
	  writer->insertRead(1);
	  writer->insertPrint(1);
	  writer->insertAssign(1);
	  writer->insertCall(1);
	  writer->insertWhile(1);
	  writer->insertIf(1);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {rArgSyn, rArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: SYN, rArg: STMT_NUM." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(SYN, 2);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::STMT_NO, "2");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {rArgSyn, rArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}
  }

  SECTION("Evaluate statement only queries with WILDCARD as left argument") {
	SECTION("WILDCARD non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: WILDCARD, rArg: WILDCARD."
				<< std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(_, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn})
	  };

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &cl : clauses) {
		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::FOLLOWS:
			std::cout << "FOLLOWS passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::FOLLOWST:
			std::cout << "FOLLOWST passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENT:
			std::cout << "PARENT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENTT:
			std::cout << "PARENTT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::NEXT:
			std::cout << "TESTING NEXT" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}
	  }
	}

	SECTION("WILDCARD empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with empty result that has lArg: WILDCARD, rArg: WILDCARD." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(_, _);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, rArgSyn}),
		  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, rArgSyn})
	  };

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {selectSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &cl : clauses) {
		// need to reset evaluator after testing each query
		evaluator = QueryEvaluator::QueryEvaluator(reader);
		q = PQL::Query({varDeclaration}, {cl}, {selectSyn});
		res = evaluator.evaluateQuery(q);
		REQUIRE(res == expectedRes);
		switch (cl.clauseType) {
		  case SimpleProgram::DesignAbstraction::FOLLOWS:
			std::cout << "FOLLOWS passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::FOLLOWST:
			std::cout << "FOLLOWST passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENT:
			std::cout << "PARENT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::PARENTT:
			std::cout << "PARENTT passed" << std::endl;
			break;
		  case SimpleProgram::DesignAbstraction::NEXT:
			std::cout << "TESTING NEXT" << std::endl;
			break;
		  default:
			std::cout << "UNKNOWN CLAUSE" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: WILDCARD, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN such that XXX(_, SYN);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);
	  writer->insertStatement(2);
	  writer->insertRead(2);
	  writer->insertPrint(2);
	  writer->insertAssign(2);
	  writer->insertCall(2);
	  writer->insertWhile(2);
	  writer->insertIf(2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, lArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {lArgSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"2"};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, syn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM non-empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: WILDCARD, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(_, SYN);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);
	  writer->insertStatement(2);
	  writer->insertRead(2);
	  writer->insertPrint(2);
	  writer->insertAssign(2);
	  writer->insertCall(2);
	  writer->insertWhile(2);
	  writer->insertIf(2);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, lArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {lArgSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, syn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: WILDCARD, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select v such that XXX(_, SYN);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto lArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, lArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {lArgSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lArgSyn, syn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lArgSyn, syn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}
  }

  SECTION("Evaluate statement only queries with WILDCARD as RIGHT argument") {
	SECTION("SYNONYM non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: WILDCARD." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN such that XXX(SYN, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);
	  writer->insertStatement(1);
	  writer->insertRead(1);
	  writer->insertPrint(1);
	  writer->insertAssign(1);
	  writer->insertCall(1);
	  writer->insertWhile(1);
	  writer->insertIf(1);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {rArgSyn, rArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {rArgSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"1"};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM non-empty ") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: WILDCARD." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN such that XXX(SYN, _);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);
	  writer->insertStatement(1);
	  writer->insertRead(1);
	  writer->insertPrint(1);
	  writer->insertAssign(1);
	  writer->insertCall(1);
	  writer->insertWhile(1);
	  writer->insertIf(1);

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };
	  auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {rArgSyn, rArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {rArgSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"testVar1"};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {selectSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("SYNONYM empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: WILDCARD." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN such that XXX(SYN, _);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  auto rArgSyn = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {rArgSyn, rArgSyn}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {rArgSyn});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &syn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {syn, rArgSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {syn, rArgSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, syn}, {cl}, {syn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << syn.identity << " passed" << std::endl;
		}
	  }
	}
  }

  SECTION("Evaluate statement only queries with 2 synonyms") {
	SECTION("LEFT SYNONYM non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN1 such that XXX(SYN1, SYN2);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);
	  writer->insertStatement({1, 2});
	  writer->insertRead({1, 2});
	  writer->insertPrint({1, 2});
	  writer->insertAssign({1, 2});
	  writer->insertCall({1, 2});
	  writer->insertWhile({1, 2});
	  writer->insertIf({1, 2});

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF1"),

		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF2")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {varDeclaration, varDeclaration}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"1"};
	  for (auto i = 0; i < 7; ++i) {
		PQL::Synonym lSyn = declarations[i];
		for (auto j = 7; j < 14; ++j) {
		  PQL::Synonym rSyn = declarations[j];
		  clauses = {
			  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lSyn, rSyn})
		  };

		  for (auto const &cl : clauses) {
			switch (cl.clauseType) {
			  case SimpleProgram::DesignAbstraction::FOLLOWS:
				std::cout << "TESTING FOLLOWS" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::FOLLOWST:
				std::cout << "TESTING FOLLOWST" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::PARENT:
				std::cout << "TESTING PARENT" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::PARENTT:
				std::cout << "TESTING PARENTT" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::NEXT:
				std::cout << "TESTING NEXT" << std::endl;
				break;
			  default:
				std::cout << "UNKNOWN CLAUSE" << std::endl;
			}

			// need to reset evaluator after testing each query
			evaluator = QueryEvaluator::QueryEvaluator(reader);
			q = PQL::Query({varDeclaration, lSyn}, {cl}, {lSyn});
			res = evaluator.evaluateQuery(q);
			REQUIRE(res == expectedRes);
			std::cout << lSyn.identity << " and " << rSyn.identity << " passed" << std::endl;
		  }
		}
	  }
	}

	SECTION("RIGHT SYNONYM non-empty and correct statements") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN1 such that XXX(SYN1, SYN2);" << std::endl;

	  writer->insertVariable("testVar1");
	  writer->insertFollows(1, 2);
	  writer->insertFollowsT(1, 2);
	  writer->insertParent(1, 2);
	  writer->insertParentT(1, 2);
	  writer->insertNext(1, 2);
	  writer->insertStatement({1, 2});
	  writer->insertRead({1, 2});
	  writer->insertPrint({1, 2});
	  writer->insertAssign({1, 2});
	  writer->insertCall({1, 2});
	  writer->insertWhile({1, 2});
	  writer->insertIf({1, 2});

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF1"),

		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF2")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {varDeclaration, varDeclaration}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {"2"};
	  for (auto i = 0; i < 7; ++i) {
		PQL::Synonym lSyn = declarations[i];
		for (auto j = 7; j < 14; ++j) {
		  PQL::Synonym rSyn = declarations[j];
		  clauses = {
			  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lSyn, rSyn})
		  };

		  for (auto const &cl : clauses) {
			switch (cl.clauseType) {
			  case SimpleProgram::DesignAbstraction::FOLLOWS:
				std::cout << "TESTING FOLLOWS" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::FOLLOWST:
				std::cout << "TESTING FOLLOWST" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::PARENT:
				std::cout << "TESTING PARENT" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::PARENTT:
				std::cout << "TESTING PARENTT" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::NEXT:
				std::cout << "TESTING NEXT" << std::endl;
				break;
			  default:
				std::cout << "UNKNOWN CLAUSE" << std::endl;
			}

			// need to reset evaluator after testing each query
			evaluator = QueryEvaluator::QueryEvaluator(reader);
			q = PQL::Query({varDeclaration, lSyn}, {cl}, {rSyn});
			res = evaluator.evaluateQuery(q);
			REQUIRE(res == expectedRes);
			std::cout << lSyn.identity << " and " << rSyn.identity << " passed" << std::endl;
		  }
		}
	  }
	}

	SECTION("LEFT SYNONYM empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN1 such that XXX(SYN1, SYN2);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF1"),

		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF2")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {varDeclaration, varDeclaration}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto i = 0; i < 7; ++i) {
		PQL::Synonym lSyn = declarations[i];
		for (auto j = 7; j < 14; ++j) {
		  PQL::Synonym rSyn = declarations[j];
		  clauses = {
			  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lSyn, rSyn})
		  };

		  for (auto const &cl : clauses) {
			switch (cl.clauseType) {
			  case SimpleProgram::DesignAbstraction::FOLLOWS:
				std::cout << "TESTING FOLLOWS" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::FOLLOWST:
				std::cout << "TESTING FOLLOWST" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::PARENT:
				std::cout << "TESTING PARENT" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::PARENTT:
				std::cout << "TESTING PARENTT" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::NEXT:
				std::cout << "TESTING NEXT" << std::endl;
				break;
			  default:
				std::cout << "UNKNOWN CLAUSE" << std::endl;
			}

			// need to reset evaluator after testing each query
			evaluator = QueryEvaluator::QueryEvaluator(reader);
			q = PQL::Query({varDeclaration, lSyn}, {cl}, {lSyn});
			res = evaluator.evaluateQuery(q);
			REQUIRE(res == expectedRes);
			std::cout << lSyn.identity << " and " << rSyn.identity << " passed" << std::endl;
		  }
		}
	  }
	}

	SECTION("LEFT SYNONYM empty and same as RIGHT SYN") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN1 such that XXX(SYN1, SYN1);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {varDeclaration, varDeclaration}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};

	  for (auto const &lSyn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lSyn, lSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lSyn, lSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lSyn, lSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lSyn, lSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lSyn, lSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, lSyn}, {cl}, {lSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << lSyn.identity << " and " << lSyn.identity << " passed" << std::endl;
		}
	  }
	}

	SECTION("RIGHT SYNONYM empty") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN1 such that XXX(SYN1, SYN1);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE1"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF1"),

		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE2"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF2")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {varDeclaration, varDeclaration}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto i = 0; i < 7; ++i) {
		PQL::Synonym lSyn = declarations[i];
		for (auto j = 7; j < 14; ++j) {
		  PQL::Synonym rSyn = declarations[j];
		  clauses = {
			  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {lSyn, rSyn}),
			  PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {lSyn, rSyn})
		  };

		  for (auto const &cl : clauses) {
			switch (cl.clauseType) {
			  case SimpleProgram::DesignAbstraction::FOLLOWS:
				std::cout << "TESTING FOLLOWS" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::FOLLOWST:
				std::cout << "TESTING FOLLOWST" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::PARENT:
				std::cout << "TESTING PARENT" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::PARENTT:
				std::cout << "TESTING PARENTT" << std::endl;
				break;
			  case SimpleProgram::DesignAbstraction::NEXT:
				std::cout << "TESTING NEXT" << std::endl;
				break;
			  default:
				std::cout << "UNKNOWN CLAUSE" << std::endl;
			}

			// need to reset evaluator after testing each query
			evaluator = QueryEvaluator::QueryEvaluator(reader);
			q = PQL::Query({varDeclaration, lSyn}, {cl}, {rSyn});
			res = evaluator.evaluateQuery(q);
			REQUIRE(res == expectedRes);
			std::cout << lSyn.identity << " and " << rSyn.identity << " passed" << std::endl;
		  }
		}
	  }
	}

	SECTION("RIGHT SYNONYM empty and same as LEFT SYN") {
	  auto pkb = std::make_shared<PKB>();
	  std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
	  std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;
	  QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

	  std::cout << "============ Start testing ============" << std::endl;
	  std::cout << "Testing PQL query with non-empty result that has lArg: SYN, rArg: SYN." << std::endl;
	  std::cout << "Testing query: variable v; Select SYN1 such that XXX(SYN1, SYN1);" << std::endl;

	  writer->insertVariable("testVar1");

	  auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
	  std::vector<PQL::Synonym> declarations = {
		  PQL::Synonym(SimpleProgram::DesignEntity::STMT, "STMT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::READ, "READ"),
		  PQL::Synonym(SimpleProgram::DesignEntity::PRINT, "PRINT"),
		  PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "ASSIGN"),
		  PQL::Synonym(SimpleProgram::DesignEntity::CALL, "CALL"),
		  PQL::Synonym(SimpleProgram::DesignEntity::WHILE, "WHILE"),
		  PQL::Synonym(SimpleProgram::DesignEntity::IF, "IF")
	  };

	  // no default constructor
	  std::vector<PQL::Clause> clauses = {
		  PQL::Clause{SimpleProgram::DesignAbstraction::FOLLOWS, {varDeclaration, varDeclaration}}};

	  // no default constructor
	  PQL::Query q = PQL::Query({varDeclaration}, {clauses[0]}, {varDeclaration});
	  std::vector<std::string> res;
	  std::vector<std::string> expectedRes = {};
	  for (auto const &rSyn : declarations) {
		clauses = {
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWS, {rSyn, rSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::FOLLOWST, {rSyn, rSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENT, {rSyn, rSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::PARENTT, {rSyn, rSyn}),
			PQL::Clause(SimpleProgram::DesignAbstraction::NEXT, {rSyn, rSyn})
		};

		for (auto const &cl : clauses) {
		  switch (cl.clauseType) {
			case SimpleProgram::DesignAbstraction::FOLLOWS:
			  std::cout << "TESTING FOLLOWS" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::FOLLOWST:
			  std::cout << "TESTING FOLLOWST" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENT:
			  std::cout << "TESTING PARENT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::PARENTT:
			  std::cout << "TESTING PARENTT" << std::endl;
			  break;
			case SimpleProgram::DesignAbstraction::NEXT:
			  std::cout << "TESTING NEXT" << std::endl;
			  break;
			default:
			  std::cout << "UNKNOWN CLAUSE" << std::endl;
		  }

		  // need to reset evaluator after testing each query
		  evaluator = QueryEvaluator::QueryEvaluator(reader);
		  q = PQL::Query({varDeclaration, rSyn}, {cl}, {rSyn});
		  res = evaluator.evaluateQuery(q);
		  REQUIRE(res == expectedRes);
		  std::cout << rSyn.identity << " and " << rSyn.identity << " passed" << std::endl;
		}
	  }
	}
  }
}

