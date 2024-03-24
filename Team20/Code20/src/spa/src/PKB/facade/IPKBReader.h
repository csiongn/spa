#pragma once

#include <vector>
#include <string>
#include <memory>

#include "SP/AST.h"

// interface for FacadeAPI to implement
class IPKBReader {

 public:
  virtual ~IPKBReader() = default;

  // Procedure
  virtual std::vector<std::string> getAllProcedures() = 0;
  virtual bool containsProcedure(std::string const &procedureName) = 0;
  virtual bool hasProcedure() = 0;

  // Variables
  virtual std::vector<std::string> getAllVariables() = 0;
  virtual bool containsVariable(std::string const &variableName) = 0;
  virtual bool hasVariable() = 0;

  // Constant
  virtual std::vector<int> getAllConstants() = 0;
  virtual bool containsConstant(int constant) = 0;
  virtual bool hasConstant() = 0;

  // Statement
  virtual std::vector<int> getAllStatementNum() = 0;
  virtual bool containsStatement(int stmtNum) = 0;
  virtual bool hasStatement() = 0;

  // Assign
  virtual std::vector<int> getAllAssignStmtNum() = 0;
  virtual bool containsAssign(int stmtNum) = 0;
  virtual bool hasAssign() = 0;

  // Read
  virtual std::vector<int> getAllReadStmtNum() = 0;
  virtual bool containsRead(int stmtNum) = 0;
  virtual bool hasRead() = 0;

  // Print
  virtual std::vector<int> getAllPrintStmtNum() = 0;
  virtual bool containsPrint(int stmtNum) = 0;
  virtual bool hasPrint() = 0;

  // Call
  virtual std::vector<int> getAllCallStmtNum() = 0;
  virtual bool containsCall(int stmtNum) = 0;
  virtual bool hasCall() = 0;

  // While
  virtual std::vector<int> getAllWhileStmtNum() = 0;
  virtual bool containsWhile(int stmtNum) = 0;
  virtual bool hasWhile() = 0;

  // If
  virtual std::vector<int> getAllIfStmtNum() = 0;
  virtual bool containsIf(int stmtNum) = 0;
  virtual bool hasIf() = 0;

  // Relationship

  // Follows
  virtual std::vector<int> getFollows(int followeeStmtNum) = 0;
  virtual std::vector<int> getFollowing(int followerStmtNum) = 0;
  virtual std::vector<int> getFolloweeStmts() = 0;
  virtual std::vector<int> getFollowerStmts() = 0;
  virtual bool containsFollows(int followeeStmtNum) = 0;
  virtual bool containsFollowing(int followerStmtNum) = 0;
  virtual bool containsFollowsRelationship(int followeeStmtNum, int followerStmtNum) = 0;
  virtual bool hasFollowsRelationship() = 0;

  // FollowsT
  virtual std::vector<int> getFollowsT(int followeeStmtNum) = 0;
  virtual std::vector<int> getFollowingT(int followerStmtNum) = 0;
  virtual std::vector<int> getFolloweeTStmts() = 0;
  virtual std::vector<int> getFollowerTStmts() = 0;
  virtual bool containsFollowsT(int followeeStmtNum) = 0;
  virtual bool containsFollowingT(int followerStmtNum) = 0;
  virtual bool containsFollowsTRelationship(int followeeStmtNum, int followerStmtNum) = 0;
  virtual bool hasFollowsTRelationship() = 0;

  // Parent
  virtual std::vector<int> getChild(int parentStmtNum) = 0;
  virtual std::vector<int> getParent(int childStmtNum) = 0;
  virtual std::vector<int> getParentStmts() = 0;
  virtual std::vector<int> getChildStmts() = 0;
  virtual bool containsParent(int parentStmtNum) = 0;
  virtual bool containsChild(int childStmtNum) = 0;
  virtual bool containsParentRelationship(int parentStmtNum, int childStmtNum) = 0;
  virtual bool hasParentRelationship() = 0;

  // ParentT
  virtual std::vector<int> getChildT(int parentStmtNum) = 0;
  virtual std::vector<int> getParentT(int childStmtNum) = 0;
  virtual std::vector<int> getParentTStmts() = 0;
  virtual std::vector<int> getChildTStmts() = 0;
  virtual bool containsParentT(int parentStmtNum) = 0;
  virtual bool containsChildT(int childStmtNum) = 0;
  virtual bool containsParentTRelationship(int parentStmtNum, int childStmtNum) = 0;
  virtual bool hasParentTRelationship() = 0;

  // Uses
  virtual std::vector<std::string> getUsesVariable(int stmtNum) = 0;
  virtual std::vector<int> getUsesStmt(std::string const &variable) = 0;
  virtual std::vector<int> getUsesStmt() = 0;
  virtual std::vector<std::string> getUsesVariable() = 0;
  virtual bool containsUsesStmt(int stmtNum) = 0;
  virtual bool containsUsesVariable(std::string const &variable) = 0;
  virtual bool containsUsesRelationship(int stmtNum, std::string const &variable) = 0;
  virtual bool hasUsesRelationship() = 0;

  // Modifies
  virtual std::vector<std::string> getModifiesVariable(int stmtNum) = 0;
  virtual std::vector<int> getModifiesStmt(std::string const &variable) = 0;
  virtual std::vector<int> getModifiesStmt() = 0;
  virtual std::vector<std::string> getModifiesVariable() = 0;
  virtual bool containsModifiesStmt(int stmtNum) = 0;
  virtual bool containsModifiesVariable(std::string const &variable) = 0;
  virtual bool containsModifiesRelationship(int stmtNum, std::string const &variable) = 0;
  virtual bool hasModifiesRelationship() = 0;

  // Procedures
  // Uses
  virtual std::vector<std::string> getUsesProcVariable(std::string const &procedureName) = 0;
  virtual std::vector<std::string> getUsesProcName(std::string const &variable) = 0;
  virtual std::vector<std::string> getUsesProcVariable() = 0;
  virtual std::vector<std::string> getUsesProcName() = 0;
  virtual bool containsUsesProc(std::string const &procedureName) = 0;
  virtual bool containsUsesProcVariable(std::string const &variable) = 0;
  virtual bool containsUsesProcRelationship(std::string const &procedureName, std::string const &variable) = 0;
  virtual bool hasUsesProcRelationship() = 0;

  // Modifies
  virtual std::vector<std::string> getModifiesProcVariable(std::string const &procedureName) = 0;
  virtual std::vector<std::string> getModifiesProcName(std::string const &variable) = 0;
  virtual std::vector<std::string> getModifiesProcVariable() = 0;
  virtual std::vector<std::string> getModifiesProcName() = 0;
  virtual bool containsModifiesProc(std::string const &procedureName) = 0;
  virtual bool containsModifiesProcVariable(std::string const &variable) = 0;
  virtual bool containsModifiesProcRelationship(std::string const &procedureName, std::string const &variable) = 0;
  virtual bool hasModifiesProcRelationship() = 0;

  // Calls Proc
  virtual std::vector<std::string> getCallsProcCaller(std::string const &callee) = 0;
  virtual std::vector<std::string> getCallsProcCallee(std::string const &caller) = 0;
  virtual std::vector<std::string> getCallsProcCaller() = 0;
  virtual std::vector<std::string> getCallsProcCallee() = 0;
  virtual bool containsCallsProcCallee(std::string const &callee) = 0;
  virtual bool containsCallsProcCaller(std::string const &caller) = 0;
  virtual bool containsCallsProcRelationship(std::string const &caller, std::string const &callee) = 0;
  virtual bool hasCallsProcRelationship() = 0;

  // CallsT Proc
  virtual std::vector<std::string> getCallsTProcCaller(std::string const &callee) = 0;
  virtual std::vector<std::string> getCallsTProcCallee(std::string const &caller) = 0;
  virtual std::vector<std::string> getCallsTProcCaller() = 0;
  virtual std::vector<std::string> getCallsTProcCallee() = 0;
  virtual bool containsCallsTProcCallee(std::string const &callee) = 0;
  virtual bool containsCallsTProcCaller(std::string const &caller) = 0;
  virtual bool containsCallsTProcRelationship(std::string const &caller, std::string const &callee) = 0;
  virtual bool hasCallsTProcRelationship() = 0;

  // Next
  virtual std::vector<int> getNext(int stmtNum) = 0;
  virtual std::vector<int> getNextReverse(int stmtNum) = 0;
  virtual std::vector<int> getNext() = 0;
  virtual std::vector<int> getNextReverse() = 0;
  virtual bool containsNext(int stmtNum) = 0;
  virtual bool containsNextReverse(int stmtNum) = 0;
  virtual bool containsNextRelationship(int stmtNum, int nextStmtNum) = 0;
  virtual bool hasNextRelationship() = 0;

  // Print
  virtual std::vector<int> getPrintStmtNum(std::string const &variable) = 0;
  virtual std::vector<std::string> getPrintVariable(int stmtNum) = 0;
  virtual std::vector<int> getPrintStmtNum() = 0;
  virtual std::vector<std::string> getPrintVariable() = 0;
  virtual bool containsPrintStmt(int stmtNum) = 0;
  virtual bool containsPrintVariable(std::string const &variable) = 0;
  virtual bool containsPrintRelationship(std::string const &variable, int stmtNum) = 0;
  virtual bool hasPrintRelationship() = 0;

  // Read
  virtual std::vector<int> getReadStmtNum(std::string const &variable) = 0;
  virtual std::vector<std::string> getReadVariable(int stmtNum) = 0;
  virtual std::vector<int> getReadStmtNum() = 0;
  virtual std::vector<std::string> getReadVariable() = 0;
  virtual bool containsReadStmt(int stmtNum) = 0;
  virtual bool containsReadVariable(std::string const &variable) = 0;
  virtual bool containsReadRelationship(std::string const &variable, int stmtNum) = 0;
  virtual bool hasReadRelationship() = 0;

  // Pattern

  // Assign
  // LHS, RHS
  // return variables that has RHS expression
  virtual std::vector<std::string> getAssignPatternLHS(size_t const &rhsExprNodeHash) = 0;
  // return expression's hash that has LHS variable
  virtual std::vector<size_t> getAssignPatternRHS(std::string const &variable) = 0;
  virtual std::vector<std::string> getAssignPatternLHS() = 0;
  // RHS hash
  virtual std::vector<size_t> getAssignPatternRHS() = 0;

  // get stmt number
  virtual std::vector<int> getAssignPatternStmtNum(std::string const &variable, size_t const &rhsExprNodeHash) = 0;
  virtual std::vector<int> getAssignPatternLHSStmtNum(std::string const &variable) = 0;
  virtual std::vector<int> getAssignPatternRHSStmtNum(size_t const &rhsExprNodeHash) = 0;

  // get expr node ptr
  virtual std::vector<std::shared_ptr<ExprNode>> getAssignPatternRHSExprNodePtr(size_t const &rhsExprNodeHash) = 0;
  virtual std::vector<std::shared_ptr<ExprNode>> getAssignPatternRHSExprNodePtr() = 0;

  // will convert expression into node hash to do comparison
  virtual bool containsAssignPattern(std::string const &variable, size_t const &rhsExprNodeHash) = 0;
  virtual bool containsAssignPatternLHS(std::string const &variable) = 0;
  virtual bool containsAssignPatternRHS(size_t const &rhsExprNodeHash) = 0;

  virtual bool hasAssignPattern() = 0;

  // Assign Partial
  // return variables that has RHS expression
  virtual std::vector<std::string> getAssignPartialPatternLHS(size_t const &rhsExprNodeHash) = 0;
  // return expression's hash that has LHS variable
  virtual std::vector<size_t> getAssignPartialPatternRHS(std::string const &variable) = 0;
  virtual std::vector<std::string> getAssignPartialPatternLHS() = 0;
  // RHS hash
  virtual std::vector<size_t> getAssignPartialPatternRHS() = 0;

  // get stmt number
  virtual std::vector<int> getAssignPartialPatternStmtNum(std::string const &variable,
														  size_t const &rhsExprNodeHash) = 0;
  virtual std::vector<int> getAssignPartialPatternLHSStmtNum(std::string const &variable) = 0;
  virtual std::vector<int> getAssignPartialPatternRHSStmtNum(size_t const &rhsExprNodeHash) = 0;

  // get expr node ptr
  virtual std::vector<std::shared_ptr<ExprNode>> getAssignPartialPatternRHSExprNodePtr(size_t const &rhsExprNodeHash) = 0;
  virtual std::vector<std::shared_ptr<ExprNode>> getAssignPartialPatternRHSExprNodePtr() = 0;

  // will convert expression into node hash to do comparison
  virtual bool containsAssignPartialPattern(std::string const &variable, size_t const &rhsExprNodeHash) = 0;
  virtual bool containsAssignPartialPatternLHS(std::string const &variable) = 0;
  virtual bool containsAssignPartialPatternRHS(size_t const &rhsExprNodeHash) = 0;

  virtual bool hasAssignPartialPattern() = 0;

  // If
  virtual std::vector<std::string> getIfPatternVariable(int stmtNum) = 0;
  virtual std::vector<int> getIfPatternStmtNum(std::string const &variable) = 0;
  virtual std::vector<std::string> getIfPatternVariable() = 0;
  virtual std::vector<int> getIfPatternStmtNum() = 0;
  virtual bool containsIfPattern(std::string const &variable, int stmtNum) = 0;
  virtual bool containsIfPatternStmt(int stmtNum) = 0;
  virtual bool containsIfPatternVariable(std::string const &variable) = 0;
  virtual bool hasIfPattern() = 0;

  // While
  virtual std::vector<std::string> getWhilePatternVariable(int stmtNum) = 0;
  virtual std::vector<int> getWhilePatternStmtNum(std::string const &variable) = 0;
  virtual std::vector<std::string> getWhilePatternVariable() = 0;
  virtual std::vector<int> getWhilePatternStmtNum() = 0;
  virtual bool containsWhilePattern(std::string const &variable, int stmtNum) = 0;
  virtual bool containsWhilePatternStmt(int stmtNum) = 0;
  virtual bool containsWhilePatternVariable(std::string const &variable) = 0;
  virtual bool hasWhilePattern() = 0;
};