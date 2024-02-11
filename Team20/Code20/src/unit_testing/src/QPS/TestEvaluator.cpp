#include "catch.hpp"

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "../TestUtils.h"

TEST_CASE("Evaluate query with only select") {

    auto pkb = std::make_shared<PKB>();
    std::shared_ptr<IPKBReader> reader = pkb->pkbFacade;
    std::shared_ptr<IPKBWriter> writer = pkb->pkbFacade;

    QueryEvaluator::QueryEvaluator evaluator = QueryEvaluator::QueryEvaluator(reader);

    SECTION("one variable only") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE with only one variable." << std::endl;
        std::cout << "Testing query: variable v; Select v;" << std::endl;

        writer->insertVariable("testVar1");

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto q = PQL::Query({varDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testVar1"};
        REQUIRE(res == expectedRes);
    }

    SECTION("multiple variables") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with multiple variables." << std::endl;
        std::cout << "Testing query: variable v; Select v;" << std::endl;

        writer->insertVariable("testVar1");
        writer->insertVariable("testVar2");
        writer->insertVariable("testVar3");
        writer->insertVariable("testVar4");
        writer->insertVariable("testVar5");


        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto q = PQL::Query({varDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {"testVar1", "testVar2", "testVar3","testVar4", "testVar5"};
        REQUIRE(checkVecValuesEqual(res, expectedRes));
    }

    SECTION("no variable") {
        std::cout << "============ Start testing ============" << std::endl;
        std::cout << "Testing SIMPLE program with no variable." << std::endl;
        std::cout << "Testing query: variable v; Select v;" << std::endl;

        auto varDeclaration = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto selectSyn = PQL::Synonym(SimpleProgram::DesignEntity::VARIABLE, "v");
        auto q = PQL::Query({varDeclaration}, {}, selectSyn);

        auto res = evaluator.evaluateQuery(q);
        std::vector<std::string> expectedRes = {};
        REQUIRE(res == expectedRes);
    }
}


