#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "IPKBReader.h"
#include "IPKBWriter.h"
#include "PKB/Database.h"

// PKBFacade implements SP and QPS Facade to provide a facade for the SPA to access the PKB.
class PKBFacade : public IPKBWriter, public IPKBReader {
 private:
  std::shared_ptr<Database> db;

 public:
  explicit PKBFacade(std::shared_ptr<Database> db);

  // Procedure
  void insertProcedure(std::string const &procedure) override;
  void insertProcedure(const std::unordered_set<std::string> &procedureSet) override;
  std::vector<std::string> getAllProcedures() override;
  bool containsProcedure(std::string const &procedure) override;
  bool hasProcedure() override;

  // Variable
  void insertVariable(std::string const &variable) override;
  void insertVariable(const std::unordered_set<std::string> &variableSet) override;
  std::vector<std::string> getAllVariables() override;
  bool containsVariable(std::string const &variable) override;
  bool hasVariable() override;

  // Constant
  void insertConstant(int constant) override;
  void insertConstant(const std::unordered_set<int> &constantSet) override;
  std::vector<int> getAllConstants() override;
  bool containsConstant(int constant) override;
  bool hasConstant() override;

  // Statement
  void insertStatement(int stmtNum) override;
  void insertStatement(const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getAllStatementNum() override;
  bool containsStatement(int stmtNum) override;
  bool hasStatement() override;

  // Assign
  void insertAssign(int stmtNum) override;
  void insertAssign(const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getAllAssignStmtNum() override;
  bool containsAssign(int stmtNum) override;
  bool hasAssign() override;

  // Read
  void insertRead(int stmtNum) override;
  void insertRead(const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getAllReadStmtNum() override;
  bool containsRead(int stmtNum) override;
  bool hasRead() override;

  // Print
  void insertPrint(int stmtNum) override;
  void insertPrint(const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getAllPrintStmtNum() override;
  bool containsPrint(int stmtNum) override;
  bool hasPrint() override;

  // Call
  void insertCall(int stmtNum) override;
  void insertCall(const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getAllCallStmtNum() override;
  bool containsCall(int stmtNum) override;
  bool hasCall() override;

  // While
  void insertWhile(int stmtNum) override;
  void insertWhile(const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getAllWhileStmtNum() override;
  bool containsWhile(int stmtNum) override;
  bool hasWhile() override;

  // If
  void insertIf(int stmtNum) override;
  void insertIf(const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getAllIfStmtNum() override;
  bool containsIf(int stmtNum) override;
  bool hasIf() override;

  // Follows Writer - Reader
  void insertFollows(int followeeStmtNum, int followerStmtNum) override;
  void insertFollows(int followeeStmtNum, const std::unordered_set<int> &followerStmtSet) override;
  std::vector<int> getFollows(int followeeStmtNum) override;
  std::vector<int> getFollowing(int followerStmtNum) override;
  std::vector<int> getFolloweeStmts() override;
  std::vector<int> getFollowerStmts() override;
  bool containsFollows(int followeeStmtNum) override;
  bool containsFollowing(int followerStmtNum) override;
  bool containsFollowsRelationship(int followeeStmtNum, int followerStmtNum) override;
  bool hasFollowsRelationship() override;

  // FollowsT Writer - Reader
  void insertFollowsT(int followeeStmtNum, int followerStmtNum) override;
  void insertFollowsT(int followeeStmtNum, const std::unordered_set<int> &followerStmtSet) override;
  std::vector<int> getFollowsT(int followeeStmtNum) override;
  std::vector<int> getFollowingT(int followerStmtNum) override;
  std::vector<int> getFolloweeTStmts() override;
  std::vector<int> getFollowerTStmts() override;
  bool containsFollowsT(int followeeStmtNum) override;
  bool containsFollowingT(int followerStmtNum) override;
  bool containsFollowsTRelationship(int followeeStmtNum, int followerStmtNum) override;
  bool hasFollowsTRelationship() override;

  // Parent Writer - Reader
  void insertParent(int parentStmtNum, int childStmtNum) override;
  void insertParent(int parentStmtNum, const std::unordered_set<int> &childStmtSet) override;
  std::vector<int> getChild(int parentStmtNum) override;
  std::vector<int> getParent(int childStmtNum) override;
  std::vector<int> getParentStmts() override;
  std::vector<int> getChildStmts() override;
  bool containsParent(int parentStmtNum) override;
  bool containsChild(int childStmtNum) override;
  bool containsParentRelationship(int parentStmtNum, int childStmtNum) override;
  bool hasParentRelationship() override;

  // ParentT Writer - Reader
  void insertParentT(int parentStmtNum, int childStmtNum) override;
  void insertParentT(int parentStmtNum, const std::unordered_set<int> &childStmtSet) override;
  std::vector<int> getChildT(int parentStmtNum) override;
  std::vector<int> getParentT(int childStmtNum) override;
  std::vector<int> getParentTStmts() override;
  std::vector<int> getChildTStmts() override;
  bool containsParentT(int parentStmtNum) override;
  bool containsChildT(int childStmtNum) override;
  bool containsParentTRelationship(int parentStmtNum, int childStmtNum) override;
  bool hasParentTRelationship() override;

  // Uses Stmt Writer - Reader
  void insertUsesStmt(int stmtNum, std::string const &variable) override;
  void insertUsesStmt(int stmtNum, std::unordered_set<std::string> const &variableSet) override;
  std::vector<std::string> getUsesVariable(int stmtNum) override;
  std::vector<int> getUsesStmt(std::string const &variable) override;
  std::vector<int> getUsesStmt() override;
  std::vector<std::string> getUsesVariable() override;
  bool containsUsesStmt(int stmtNum) override;
  bool containsUsesVariable(std::string const &variable) override;
  bool containsUsesRelationship(int stmtNum, std::string const &variable) override;
  bool hasUsesRelationship() override;

  // Modifies Stmt Writer - Reader
  void insertModifiesStmt(int stmtNum, std::string const &variable) override;
  void insertModifiesStmt(int stmtNum, std::unordered_set<std::string> const &variableSet) override;
  std::vector<std::string> getModifiesVariable(int stmtNum) override;
  std::vector<int> getModifiesStmt(std::string const &variable) override;
  std::vector<int> getModifiesStmt() override;
  std::vector<std::string> getModifiesVariable() override;
  bool containsModifiesStmt(int stmtNum) override;
  bool containsModifiesVariable(std::string const &variable) override;
  bool containsModifiesRelationship(int stmtNum, std::string const &variable) override;
  bool hasModifiesRelationship() override;

  // Procedures
  // Uses Proc
  void insertUsesProc(std::string const &procedureName, std::string const &variable) override;
  void insertUsesProc(std::string const &procedureName, std::unordered_set<std::string> const &variableSet) override;
  std::vector<std::string> getUsesProcVariable(std::string const &procedureName) override;
  std::vector<std::string> getUsesProcName(std::string const &variable) override;
  std::vector<std::string> getUsesProcVariable() override;
  std::vector<std::string> getUsesProcName() override;
  bool containsUsesProc(std::string const &procedureName) override;
  bool containsUsesProcVariable(std::string const &variable) override;
  bool containsUsesProcRelationship(std::string const &procedureName, std::string const &variable) override;
  bool hasUsesProcRelationship() override;

  // Modifies Proc
  void insertModifiesProc(std::string const &procedureName, std::string const &variable) override;
  void insertModifiesProc(std::string const &procedureName,
						  std::unordered_set<std::string> const &variableSet) override;
  std::vector<std::string> getModifiesProcVariable(std::string const &procedureName) override;
  std::vector<std::string> getModifiesProcName(std::string const &variable) override;
  std::vector<std::string> getModifiesProcVariable() override;
  std::vector<std::string> getModifiesProcName() override;
  bool containsModifiesProc(std::string const &procedureName) override;
  bool containsModifiesProcVariable(std::string const &variable) override;
  bool containsModifiesProcRelationship(std::string const &procedureName, std::string const &variable) override;
  bool hasModifiesProcRelationship() override;

  // Calls Proc
  void insertCallsProc(std::string const &caller, std::string const &callee) override;
  void insertCallsProc(std::string const &caller, std::unordered_set<std::string> const &calleeSet) override;
  std::vector<std::string> getCallsProcCaller(std::string const &callee) override;
  std::vector<std::string> getCallsProcCallee(std::string const &caller) override;
  std::vector<std::string> getCallsProcCaller() override;
  std::vector<std::string> getCallsProcCallee() override;
  bool containsCallsProcCallee(std::string const &callee) override;
  bool containsCallsProcCaller(std::string const &caller) override;
  bool containsCallsProcRelationship(std::string const &caller, std::string const &callee) override;
  bool hasCallsProcRelationship() override;

  // CallsT Proc
  void insertCallsTProc(std::string const &caller, std::string const &callee) override;
  void insertCallsTProc(std::string const &caller, std::unordered_set<std::string> const &calleeSet) override;
  std::vector<std::string> getCallsTProcCaller(std::string const &callee) override;
  std::vector<std::string> getCallsTProcCallee(std::string const &caller) override;
  std::vector<std::string> getCallsTProcCaller() override;
  std::vector<std::string> getCallsTProcCallee() override;
  bool containsCallsTProcCallee(std::string const &callee) override;
  bool containsCallsTProcCaller(std::string const &caller) override;
  bool containsCallsTProcRelationship(std::string const &caller, std::string const &callee) override;
  bool hasCallsTProcRelationship() override;

  // Next
  void insertNext(int stmtNum, int nextStmtNum) override;
  void insertNext(int stmtNum, const std::unordered_set<int> &nextStmtSet) override;
  std::vector<int> getNext(int stmtNum) override;
  std::vector<int> getNextReverse(int stmtNum) override;
  std::vector<int> getNext() override;
  std::vector<int> getNextReverse() override;
  bool containsNext(int stmtNum) override;
  bool containsNextReverse(int stmtNum) override;
  bool containsNextRelationship(int stmtNum, int nextStmtNum) override;
  bool hasNextRelationship() override;

  // Print
  void insertPrintVar(std::string const &variable, const int &stmtNum) override;
  void insertPrintVar(std::string const &variable, const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getPrintStmtNum(std::string const &variable) override;
  std::vector<std::string> getPrintVariable(int stmtNum) override;
  std::vector<int> getPrintStmtNum() override;
  std::vector<std::string> getPrintVariable() override;
  bool containsPrintStmt(int stmtNum) override;
  bool containsPrintVariable(std::string const &variable) override;
  bool containsPrintRelationship(std::string const &variable, int stmtNum) override;
  bool hasPrintRelationship() override;

  // Read
  void insertReadVar(std::string const &variable, const int &stmtNum) override;
  void insertReadVar(std::string const &variable, const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getReadStmtNum(std::string const &variable) override;
  std::vector<std::string> getReadVariable(int stmtNum) override;
  std::vector<int> getReadStmtNum() override;
  std::vector<std::string> getReadVariable() override;
  bool containsReadStmt(int stmtNum) override;
  bool containsReadVariable(std::string const &variable) override;
  bool containsReadRelationship(std::string const &variable, int stmtNum) override;
  bool hasReadRelationship() override;

  // CallsProcStmt
  void insertCallsProcStmt(std::string const &procName, const int &stmtNum) override;
  void insertCallsProcStmt(std::string const &procName, const std::unordered_set<int> &stmtSet) override;
  std::vector<int> getCallsProcStmtNum(std::string const &callsProcName) override;
  std::vector<std::string> getCallsProcName(int stmtNum) override;
  std::vector<int> getCallsProcStmtNum() override;
  std::vector<std::string> getCallsProcName() override;
  bool containsCallsProcStmt(int stmtNum) override;
  bool containsCallsProcName(std::string const &callsProcName) override;
  bool containsCallsProcStmtRelationship(std::string const &callsProcName, int stmtNum) override;
  bool hasCallsProcStmtRelationship() override;

  // Assign Pattern
  void insertAssignPattern(std::string const &lhsVar, const size_t &rhsExprNodeHash, const int &stmtNum,
						   const std::shared_ptr<ExprNode> &nodePtr) override;

  std::vector<std::string> getAssignPatternLHS(size_t const &rhsExprNodeHash) override;
  std::vector<size_t> getAssignPatternRHS(std::string const &variable) override;
  std::vector<std::string> getAssignPatternLHS() override;
  std::vector<size_t> getAssignPatternRHS() override;

  // get stmt number
  std::vector<int> getAssignPatternStmtNum(std::string const &variable, size_t const &rhsExprNodeHash) override;
  std::vector<int> getAssignPatternLHSStmtNum(std::string const &variable) override;
  std::vector<int> getAssignPatternRHSStmtNum(size_t const &rhsExprNodeHash) override;

  std::vector<std::shared_ptr<ExprNode>> getAssignPatternRHSExprNodePtr(size_t const &rhsExprNodeHash) override;
  // get all assign pattern RHS node ptrs
  std::vector<std::shared_ptr<ExprNode>> getAssignPatternRHSExprNodePtr() override;

  bool containsAssignPattern(std::string const &variable, size_t const &rhsExprNodeHash) override;
  bool containsAssignPatternLHS(std::string const &variable) override;
  bool containsAssignPatternRHS(size_t const &rhsExprNodeHash) override;
  bool hasAssignPattern() override;

  // AssignPartial Pattern
  void insertAssignPartialPattern(std::string const &lhsVar, const size_t &rhsExprNodeHash, const int &stmtNum,
								  const std::shared_ptr<ExprNode> &nodePtr) override;

  std::vector<std::string> getAssignPartialPatternLHS(size_t const &rhsExprNodeHash) override;
  std::vector<size_t> getAssignPartialPatternRHS(std::string const &variable) override;
  std::vector<std::string> getAssignPartialPatternLHS() override;
  std::vector<size_t> getAssignPartialPatternRHS() override;

  // get stmt number
  std::vector<int> getAssignPartialPatternStmtNum(std::string const &variable, size_t const &rhsExprNodeHash) override;
  std::vector<int> getAssignPartialPatternLHSStmtNum(std::string const &variable) override;
  std::vector<int> getAssignPartialPatternRHSStmtNum(size_t const &rhsExprNodeHash) override;

  std::vector<std::shared_ptr<ExprNode>> getAssignPartialPatternRHSExprNodePtr(size_t const &rhsExprNodeHash) override;
  // get all AssignPartial pattern RHS node ptrs
  std::vector<std::shared_ptr<ExprNode>> getAssignPartialPatternRHSExprNodePtr() override;

  bool containsAssignPartialPattern(std::string const &variable, size_t const &rhsExprNodeHash) override;
  bool containsAssignPartialPatternLHS(std::string const &variable) override;
  bool containsAssignPartialPatternRHS(size_t const &rhsExprNodeHash) override;
  bool hasAssignPartialPattern() override;

  // If Pattern
  void insertIfPattern(std::string const &variable, const int &stmtNum) override;
  void insertIfPattern(std::string const &variable, const std::unordered_set<int> &stmtSet) override;

  std::vector<std::string> getIfPatternVariable(int stmtNum) override;
  std::vector<int> getIfPatternStmtNum(std::string const &variable) override;
  std::vector<std::string> getIfPatternVariable() override;
  std::vector<int> getIfPatternStmtNum() override;
  bool containsIfPattern(std::string const &variable, int stmtNum) override;
  bool containsIfPatternStmt(int stmtNum) override;
  bool containsIfPatternVariable(std::string const &variable) override;
  bool hasIfPattern() override;

  // While Pattern
  void insertWhilePattern(std::string const &variable, const int &stmtNum) override;
  void insertWhilePattern(std::string const &variable, const std::unordered_set<int> &stmtSet) override;

  std::vector<std::string> getWhilePatternVariable(int stmtNum) override;
  std::vector<int> getWhilePatternStmtNum(std::string const &variable) override;
  std::vector<std::string> getWhilePatternVariable() override;
  std::vector<int> getWhilePatternStmtNum() override;
  bool containsWhilePattern(std::string const &variable, int stmtNum) override;
  bool containsWhilePatternStmt(int stmtNum) override;
  bool containsWhilePatternVariable(std::string const &variable) override;
  bool hasWhilePattern() override;
};
