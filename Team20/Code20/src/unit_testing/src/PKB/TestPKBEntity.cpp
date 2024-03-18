#include "catch.hpp"

#include "../TestUtils.h"
#include "PKB/Database.h"
#include "PKB/PKB.h"
#include "PKB/manager/entity/EntityManager.h"

TEST_CASE("PKB Entity Test") {
  auto pkb = std::make_shared<PKB>();
  auto pkbFacade = pkb->pkbFacade;


  // int type: Assign, statement, while, if, call, print, read, constant
  SECTION("Test PKB - EntityManager<int>") {
	// Create an instance of PKB
	auto intEntityManager = std::make_shared<EntityManager<int>>();


	// insert
	intEntityManager->insert(1);
	intEntityManager->insert(2);
	intEntityManager->insert(3);
	// insert duplicate
	intEntityManager->insert(1);

	// a can be assign, statement, while, if, call, print, read
	// Query: Select a
	// get
	auto values = intEntityManager->get();
	std::vector<int> expectedValues = {1, 2, 3};
	REQUIRE(checkVecValuesEqual(values, expectedValues));
	REQUIRE(intEntityManager->contains(1) == true);
  }

	// string type: procedure, variable
  SECTION("EntityManager<string>") {
	// Create an instance of PKB
	auto stringEntityManager = std::make_shared<EntityManager<std::string>>();
	auto procedureManager = std::make_shared<ProcedureManager>();

	// insert
	stringEntityManager->insert("main");
	stringEntityManager->insert("function1");
	stringEntityManager->insert("function2");

	procedureManager->insert("main");
	procedureManager->insert("function1");
	procedureManager->insert("function2");

	// insert duplicate
	stringEntityManager->insert("main");

	// a can be procedure, variable
	// Query: Select a
	// get
	auto stringEntityVec = stringEntityManager->get();
	auto procedureVec = procedureManager->get();
	REQUIRE(checkVecValuesEqual(stringEntityVec, procedureVec));
	REQUIRE(stringEntityManager->contains("main") == true);
	REQUIRE(procedureManager->contains("hello") == false);
  }

  SECTION("PKBFACADE - AssignManager") {

	// hasAssign
	bool contains = pkbFacade->hasAssign();
	REQUIRE(contains == false);

	// insertAssign
	pkbFacade->insertAssign(1);
	pkbFacade->insertAssign({4, 5, 80});

	// getAllAssignStmtNum
	auto assignVec = pkbFacade->getAllAssignStmtNum();
	REQUIRE(checkVecValuesEqual(assignVec, {1, 4, 5, 80}));

	// containsAssign
	bool containsAssign = pkbFacade->containsAssign(1000);
	REQUIRE(containsAssign == false);

	// hasAssign
	contains = pkbFacade->hasAssign();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - CallManager") {

	// hasCall
	bool contains = pkbFacade->hasCall();
	REQUIRE(contains == false);

	// insertCall
	pkbFacade->insertCall(1);
	// insert duplicated
	pkbFacade->insertCall({1, 3});

	// getAllCallStmtNum
	auto callVec = pkbFacade->getAllCallStmtNum();
	REQUIRE(checkVecValuesEqual(callVec, {1, 3}));

	// containsCall
	bool containsCall = pkbFacade->containsCall(5);
	REQUIRE(containsCall == false);

	// hasCall
	contains = pkbFacade->hasCall();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - ConstantManager") {
	// hasConstant
	bool contains = pkbFacade->hasConstant();
	REQUIRE(contains == false);

	// insertConstant
	pkbFacade->insertConstant(1);
	// insert duplicated
	pkbFacade->insertConstant({1, 3});

	// getAllConstant
	auto constantVec = pkbFacade->getAllConstants();
	REQUIRE(checkVecValuesEqual(constantVec, {1, 3}));

	// containsConstant
	bool containsConstant = pkbFacade->containsConstant(5);
	REQUIRE(containsConstant == false);

	// hasConstant
	contains = pkbFacade->hasConstant();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - IfManager") {
	// hasIf
	bool contains = pkbFacade->hasIf();
	REQUIRE(contains == false);

	// insertIf
	pkbFacade->insertIf(1);
	// insert duplicated
	pkbFacade->insertIf({1, 3});

	// getAllIfStmtNum
	auto ifVec = pkbFacade->getAllIfStmtNum();
	REQUIRE(checkVecValuesEqual(ifVec, {1, 3}));

	// containsIf
	bool containsIf = pkbFacade->containsIf(1);
	REQUIRE(containsIf == true);

	// hasIf
	contains = pkbFacade->hasIf();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - PrintManager") {
	// hasPrint
	bool contains = pkbFacade->hasPrint();
	REQUIRE(contains == false);

	// insertPrint
	pkbFacade->insertPrint(5);
	// insert duplicated
	pkbFacade->insertPrint({1, 2, 3, 5});

	// getAllPrintStmtNum
	auto printVec = pkbFacade->getAllPrintStmtNum();
	REQUIRE(checkVecValuesEqual(printVec, {1, 2, 3, 5}));

	// containsPrint
	bool containsPrint = pkbFacade->containsPrint(1);
	REQUIRE(containsPrint == true);

	// hasPrint
	contains = pkbFacade->hasPrint();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - ProcedureManager") {
	// hasProcedure
	bool contains = pkbFacade->hasProcedure();
	REQUIRE(contains == false);

	// insertProcedure
	pkbFacade->insertProcedure("main");
	// insert duplicated
	pkbFacade->insertProcedure({"main", "function1", "function2"});

	// getAllProcedures
	auto procedureVec = pkbFacade->getAllProcedures();
	REQUIRE(checkVecValuesEqual(procedureVec, {"main", "function1", "function2"}));

	// containsProcedure
	bool containsProcedure = pkbFacade->containsProcedure("hello");
	REQUIRE(containsProcedure == false);
	// hasProcedure
	contains = pkbFacade->hasProcedure();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - ReadManager") {
	// hasRead
	bool contains = pkbFacade->hasRead();
	REQUIRE(contains == false);

	// insertRead
	pkbFacade->insertRead(5);
	// insert duplicated
	pkbFacade->insertRead({1, 2, 5, 6});

	// getAllReadStmtNum
	auto readVec = pkbFacade->getAllReadStmtNum();
	REQUIRE(checkVecValuesEqual(readVec, {1, 2, 5, 6}));

	// containsRead
	bool containsRead = pkbFacade->containsRead(100);
	REQUIRE(containsRead == false);

	// hasRead
	contains = pkbFacade->hasRead();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - StatementManager") {
	// hasStatement
	bool contains = pkbFacade->hasStatement();
	REQUIRE(contains == false);

	// insertStatement
	pkbFacade->insertStatement(5);
	// insert duplicated
	pkbFacade->insertStatement({1, 2, 3, 4, 5, 6});

	// getAllStatement
	auto statementVec = pkbFacade->getAllStatementNum();
	REQUIRE(checkVecValuesEqual(statementVec, {1, 2, 3, 4, 5, 6}));

	// containsStatement
	bool containsStatement = pkbFacade->containsStatement(100);
	REQUIRE(containsStatement == false);

	// hasStatement
	contains = pkbFacade->hasStatement();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - VariableManager") {
	// hasVariable
	bool contains = pkbFacade->hasVariable();
	REQUIRE(contains == false);

	// insertVariable
	pkbFacade->insertVariable("a");
	// insert duplicated
	pkbFacade->insertVariable({"a", "b", "c"});

	// getAllVariables
	auto variableVec = pkbFacade->getAllVariables();
	REQUIRE(checkVecValuesEqual(variableVec, {"a", "b", "c"}));

	// containsVariable
	bool containsVariable = pkbFacade->containsVariable("hello");
	REQUIRE(containsVariable == false);

	// hasVariable
	contains = pkbFacade->hasVariable();
	REQUIRE(contains == true);
  }

  SECTION("PKBFACADE - WhileManager") {
	// hasWhile
	bool contains = pkbFacade->hasWhile();
	REQUIRE(contains == false);

	// insertWhile
	pkbFacade->insertWhile(5);
	// insert duplicated
	pkbFacade->insertWhile({1, 2, 3, 4, 5, 6});

	// getAllWhileStmtNum
	auto whileVec = pkbFacade->getAllWhileStmtNum();
	REQUIRE(checkVecValuesEqual(whileVec, {1, 2, 3, 4, 5, 6}));

	// containsWhile
	bool containsWhile = pkbFacade->containsWhile(100);
	REQUIRE(containsWhile == false);

	// hasWhile
	contains = pkbFacade->hasWhile();
	REQUIRE(contains == true);
  }
}