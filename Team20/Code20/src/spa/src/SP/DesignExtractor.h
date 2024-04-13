#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "AST.h"
#include "CallGraph.h"
#include "PKB/facade/IPKBWriter.h"

class DesignExtractor {
  // Stores code context during traversal of Source program
  class SourceContext {
   public:
    uint16_t currentStmtNumber = 0; // Keeps track of the statement numbers as we traverse
    std::string procName; // Keep track of current procedure as we traverse
  };

 public:
  explicit DesignExtractor(std::shared_ptr<IPKBWriter> pkbWriter) : pkbWriter(std::move(pkbWriter)) {
  }

  void extractDesign(const ProgramNode& astRoot);

  // Getters
  const std::unordered_map<int, int>& getFollows() const {
    return follows;
  }

  const std::unordered_map<int, std::unordered_set<int>>& getFollowsT() const {
    return followsT;
  }

  const std::unordered_map<int, int>& getParent() const {
    return parent;
  }

  const std::unordered_map<int, std::unordered_set<int>>& getParentT() const {
    return parentT;
  }

  std::unordered_map<int, std::unordered_set<std::string>>& getUses() {
    return uses;
  }

  const std::unordered_map<std::string, std::unordered_set<std::string>>& getProcUses() const {
    return procsUses;
  }

  std::unordered_map<int, std::unordered_set<std::string>>& getModifies() {
    return modifies;
  }

  const std::unordered_map<std::string, std::unordered_set<std::string>>& getProcModifies() const {
    return procsModifies;
  }

  const std::unordered_map<std::string, std::unordered_set<std::string>>& getCall() const {
    return call;
  }

  const std::unordered_map<std::string, std::unordered_set<std::string>>& getCallT() const {
    return callT;
  }

  const std::unordered_map<int, std::unordered_set<std::string>>& getIfStmts() const {
    return ifStmts;
  }

  const std::unordered_map<int, std::unordered_set<std::string>>& getWhileStmts() const {
    return whileStmts;
  }

  const std::unordered_set<std::string>& getVariables() const {
    return variables;
  }

 private:
  std::shared_ptr<IPKBWriter> pkbWriter;
  std::unique_ptr<SourceContext> ctxt = std::make_unique<SourceContext>();
  std::shared_ptr<CallGraph> callGraph = std::make_shared<CallGraph>();

  // Design abstractions
  std::unordered_map<int, int> follows;
  std::unordered_map<int, std::unordered_set<int>> followsT;
  std::unordered_map<int, int> parent;
  std::unordered_map<int, std::unordered_set<int>> parentT;
  std::unordered_map<int, std::unordered_set<std::string>> uses;
  std::unordered_map<int, std::unordered_set<std::string>> modifies;
  std::unordered_map<std::string, std::unordered_set<std::string>> procsUses;
  std::unordered_map<std::string, std::unordered_set<std::string>> procsModifies;
  std::unordered_map<std::string, std::unordered_set<std::string>> call;
  std::unordered_map<std::string, std::unordered_set<std::string>> revCall;
  std::unordered_map<std::string, std::unordered_set<std::string>> callT;

  // Entities
  std::unordered_set<std::string> procedures;
  std::unordered_set<std::string> variables;
  std::unordered_set<std::string> literals;
  std::unordered_set<int> stmts;
  std::unordered_set<int> assignStmts;
  std::unordered_map<int, std::string> callStmts;
  std::unordered_map<int, std::string> readStmts;
  std::unordered_map<int, std::string> printStmts;
  std::unordered_map<int, std::unordered_set<std::string>> ifStmts;
  std::unordered_map<int, std::unordered_set<std::string>> whileStmts;

  // Specific Nodes
  std::unordered_set<std::shared_ptr<AssignNode>> assignNodes;

  // Methods to traverse the AST
  void visitProgramNode(const ProgramNode& node);
  void visitProcedureNode(const ProcedureNode& node);
  void visitBlockNode(const BlockNode& node, int parentStmt, std::vector<int>& stmtList);
  void visitStmtNode(const StmtNode& node, int parentStmt, std::vector<int>& stmtList);
  void visitIfNode(const IfNode& node, int stmtNumber);
  void visitWhileNode(const WhileNode& node, int stmtNumber);
  void visitExprNode(const ExprNode& node, int stmtNumber);

  // Helper Methods
  void extractVariables(const ExprNode& node, std::unordered_set<std::string>& variables);

  // Utility Methods
  void updateFollows(int stmtNumber, std::vector<int>& stmtList);
  void updateParent(int childStmtNumber, int parentStmtNumber);
  void updateUses(int stmtNumber, const std::string& variableName);
  void updateModifies(int stmtNumber, const std::string& variableName);
  void updateCall(const std::string& callingProc, const std::string& calledProc);

  void insertProcedure(const std::string& procName);
  void insertVariable(const std::string& var);
  void insertLiteral(const std::string& var);
  void insertStmt(const int stmtNum);
  void insertAssign(const int stmtNum);
  void insertCall(const int stmtNum, const std::string& calledProc);
  void insertIf(const int stmtNum, std::unordered_set<std::string> controlVariables);
  void insertRead(const int stmtNum, const std::string& variable);
  void insertPrint(const int stmtNum, const std::string& variable);
  void insertWhile(const int stmtNum, std::unordered_set<std::string> controlVariables);

  void pushToPKB();
};
