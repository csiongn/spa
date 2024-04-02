#include <exception>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "PKB/facade/IPKBWriter.h"
#include "SP/RuntimeExtractor.h"

class TestException final : public std::exception {
	// https://www.geeksforgeeks.org/how-to-throw-custom-exception-in-cpp/
	// https://stackoverflow.com/questions/41753358/creating-custom-exceptions-in-c
	std::string message;
 public:
	explicit TestException(const std::string &msg) : message(msg) {}

	const char *what() const noexcept override {
		return message.c_str();
	}
};

class MockPKBWriter : public IPKBWriter {
 public:
  // Stored entities and relations
  std::unordered_set<std::string> insertedProcedures;
  std::unordered_set<std::string> insertedVariables;
  std::unordered_set<int> insertedConstants;
  std::unordered_set<int> insertedStatements;
  std::unordered_set<int> insertedAssigns;
  std::unordered_set<int> insertedReads;
  std::unordered_set<int> insertedPrints;
  std::unordered_set<int> insertedCalls;
  std::unordered_set<int> insertedWhiles;
  std::unordered_set<int> insertedIfs;
  std::vector<std::pair<int, int>> followsRelations;
  std::vector<std::pair<int, int>> followsTRelations;
  std::vector<std::pair<int, int>> parentRelations;
  std::vector<std::pair<int, int>> parentTRelations;
  std::vector<std::pair<int, const std::string>> usesRelations;
  std::unordered_map<std::string, std::unordered_set<std::string>> procUsesRelations;
  std::vector<std::pair<int, const std::string>> modifiesRelations;
  std::unordered_map<std::string, std::unordered_set<std::string>> procModifiesRelations;
  std::vector<std::pair<std::string, std::string>> callRelations;
  std::vector<std::pair<std::string, std::string>> callTRelations;
  std::vector<std::pair<int, int>> nextRelations;
  std::vector<std::pair<const std::string, int>> printVarRelations;
  std::vector<std::pair<const std::string, int>> readVarRelations;
  std::vector<std::pair<const std::string, int>> callsProcStmtRelations;
  std::vector<std::tuple<std::string, size_t, int, std::shared_ptr<ExprNode>>> assignPatterns;
  std::vector<std::tuple<std::string, size_t, int, std::shared_ptr<ExprNode>>> assignPartialPatterns;
  std::vector<std::pair<const std::string, const int>> ifPatterns;
  std::vector<std::pair<const std::string, const int>> whilePatterns;
  std::shared_ptr<RuntimeExtractor> runtimeExtractor;

  void insertProcedure(std::string const &procedureName) override {
	insertedProcedures.insert(procedureName);
  }

  void insertProcedure(const std::unordered_set<std::string> &procedureSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertVariable(std::string const &variableName) override {
	insertedVariables.insert(variableName);
  }

  void insertVariable(const std::unordered_set<std::string> &variableSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertConstant(int constant) override {
	insertedConstants.insert(constant);
  }

  void insertConstant(const std::unordered_set<int> &constantSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertStatement(int stmtNum) override {
	insertedStatements.insert(stmtNum);
  }

  void insertStatement(const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertAssign(int stmtNum) override {
	insertedAssigns.insert(stmtNum);
  }

  void insertAssign(const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertRead(int stmtNum) override {
	insertedReads.insert(stmtNum);
  }

  void insertRead(const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertPrint(int stmtNum) override {
	insertedPrints.insert(stmtNum);
  }

  void insertPrint(const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertCall(int stmtNum) override {
	insertedCalls.insert(stmtNum);
  }

  void insertCall(const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertWhile(int stmtNum) override {
	insertedWhiles.insert(stmtNum);
  }

  void insertWhile(const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertIf(int stmtNum) override {
	insertedIfs.insert(stmtNum);
  }

  void insertIf(const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertFollows(int followeeStmtNum, int followerStmtNum) override {
	followsRelations.emplace_back(followeeStmtNum, followerStmtNum);
  }

  void insertFollows(int followeeStmtNum, const std::unordered_set<int> &followerStmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertFollowsT(int followeeStmtNum, int followerStmtNum) override {
	followsTRelations.emplace_back(followeeStmtNum, followerStmtNum);
  }

  void insertFollowsT(int followeeStmtNum, const std::unordered_set<int> &followerStmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertParent(int parentStmtNum, int childStmtNum) override {
	parentRelations.emplace_back(parentStmtNum, childStmtNum);
  }

  void insertParent(int parentStmtNum, const std::unordered_set<int> &childStmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertParentT(int parentStmtNum, int childStmtNum) override {
	parentTRelations.emplace_back(parentStmtNum, childStmtNum);
  }

  void insertParentT(int parentStmtNum, const std::unordered_set<int> &childStmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertUsesStmt(int stmtNum, const std::string &variable) override {
	usesRelations.emplace_back(stmtNum, variable);
  }

  void insertUsesStmt(int stmtNum, std::unordered_set<std::string> const &variableSet) override {
	for (auto variable : variableSet) {
	  usesRelations.emplace_back(stmtNum, variable);
	}
  }

  void insertUsesProc(std::string const &procedureName, std::string const &variable) override {
	procUsesRelations[procedureName].insert(variable);
  }

  void insertUsesProc(std::string const &procedureName, std::unordered_set<std::string> const &variableSet) override {
	for (auto variable : variableSet) {
	  procUsesRelations[procedureName].insert(variable);
	}
  }

  void insertModifiesStmt(int stmtNum, std::string const &variable) override {
	modifiesRelations.emplace_back(stmtNum, variable);
  }

  void insertModifiesStmt(int stmtNum, std::unordered_set<std::string> const &variableSet) override {
	for (auto variable : variableSet) {
	  modifiesRelations.emplace_back(stmtNum, variable);
	}
  }

  void insertModifiesProc(std::string const &procedureName, std::string const &variable) override {
	procModifiesRelations[procedureName].insert(variable);
  }

  void insertModifiesProc(std::string const &procedureName,
						  std::unordered_set<std::string> const &variableSet) override {
	for (auto variable : variableSet) {
	  procModifiesRelations[procedureName].insert(variable);
	}
  }

  void insertCallsProc(std::string const &caller, std::string const &callee) override {
	callRelations.emplace_back(caller, callee);
  }

  void insertCallsProc(std::string const &caller, std::unordered_set<std::string> const &calleeSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertCallsTProc(std::string const &caller, std::string const &callee) override {
	callTRelations.emplace_back(caller, callee);
  }

  void insertCallsTProc(std::string const &caller, std::unordered_set<std::string> const &calleeSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertNext(int stmtNum, int nextStmtNum) override {
	nextRelations.emplace_back(stmtNum, nextStmtNum);
  }

  void insertNext(int stmtNum, std::unordered_set<int> const &nextStmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertPrintVar(std::string const &variable, const int &stmtNum) override {
	printVarRelations.emplace_back(variable, stmtNum);
  }

  void insertPrintVar(std::string const &variable, const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertReadVar(std::string const &variable, const int &stmtNum) override {
	readVarRelations.emplace_back(variable, stmtNum);
  }

  void insertReadVar(std::string const &variable, const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertCallsProcStmt(std::string const &procName, const int &stmtNum) override {
	callsProcStmtRelations.emplace_back(procName, stmtNum);
  }

  void insertCallsProcStmt(std::string const &procName, const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertIfPattern(std::string const &variable, const int &stmtNum) override {
	ifPatterns.emplace_back(variable, stmtNum);
  }

  void insertIfPattern(std::string const &variable, const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertWhilePattern(std::string const &variable, const int &stmtNum) override {
	whilePatterns.emplace_back(variable, stmtNum);
  }

  void insertWhilePattern(std::string const &variable, const std::unordered_set<int> &stmtSet) override {
	throw TestException("Method not supposed to be called");
  }

  void insertAssignPattern(std::string const &lhsVar,
						   const size_t &rhsExprNodeHash,
						   const int &stmtNum,
						   const std::shared_ptr<ExprNode> &nodePtr) override {
	assignPatterns.emplace_back(lhsVar, rhsExprNodeHash, stmtNum, nodePtr);
  }

  void insertAssignPartialPattern(std::string const &lhsVar,
								  const size_t &rhsExprNodeHash,
								  const int &stmtNum,
								  const std::shared_ptr<ExprNode> &nodePtr) override {
	assignPartialPatterns.emplace_back(lhsVar, rhsExprNodeHash, stmtNum, nodePtr);
  }

  void insertRuntimeExtractor(std::shared_ptr<RuntimeExtractor> runtimeExtractor) {
	this->runtimeExtractor = runtimeExtractor;
  }
};
