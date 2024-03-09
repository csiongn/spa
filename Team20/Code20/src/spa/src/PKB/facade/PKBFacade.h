#pragma once

#include "IPKBReader.h"
#include "IPKBWriter.h"
#include "PKB/Database.h"

// PKBFacade implements SP and QPS Facade to provide a facade for the SPA to access the PKB.
class PKBFacade: public IPKBWriter, public IPKBReader {
private:
    std::shared_ptr<Database> db;

public:
    explicit PKBFacade(std::shared_ptr<Database> db);

    // Procedure
    void insertProcedure(std::string const & procedure) override;
    void insertProcedure(const std::unordered_set<std::string>& procedureSet) override;
    std::vector<std::string> getAllProcedures() override;
    bool containsProcedure(std::string const & procedure) override;

    // Variable
    void insertVariable(std::string const & variable) override;
    void insertVariable(const std::unordered_set<std::string>& variableSet) override;
    std::vector<std::string> getAllVariables() override;
    bool containsVariable(std::string const & variable) override;

    // Constant
    void insertConstant(int constant) override;
    void insertConstant(const std::unordered_set<int>& constantSet) override;
    std::vector<int> getAllConstants() override;
    bool containsConstant(int constant) override;

    // Statement
    void insertStatement(int stmtNum) override;
    void insertStatement(const std::unordered_set<int>& stmtSet) override;
    std::vector<int> getAllStatementNum() override;
    bool containsStatement(int stmtNum) override;


    // Assign
    void insertAssign(int stmtNum) override;
    void insertAssign(const std::unordered_set<int>& stmtSet) override;
    std::vector<int> getAllAssignStmtNum() override;
    bool containsAssign(int stmtNum) override;

    // Read
    void insertRead(int stmtNum) override;
    void insertRead(const std::unordered_set<int>& stmtSet) override;
    std::vector<int> getAllReadStmtNum() override;
    bool containsRead(int stmtNum) override;

    // Print
    void insertPrint(int stmtNum) override;
    void insertPrint(const std::unordered_set<int>& stmtSet) override;
    std::vector<int> getAllPrintStmtNum() override;
    bool containsPrint(int stmtNum) override;

    // Call
    void insertCall(int stmtNum) override;
    void insertCall(const std::unordered_set<int>& stmtSet) override;
    std::vector<int> getAllCallStmtNum() override;
    bool containsCall(int stmtNum) override;

    // While
    void insertWhile(int stmtNum) override;
    void insertWhile(const std::unordered_set<int>& stmtSet) override;
    std::vector<int> getAllWhileStmtNum() override;
    bool containsWhile(int stmtNum) override;

    // If
    void insertIf(int stmtNum) override;
    void insertIf(const std::unordered_set<int>& stmtSet) override;
    std::vector<int> getAllIfStmtNum() override;
    bool containsIf(int stmtNum) override;

    // Follows Writer - Reader
    void insertFollows(int followeeStmtNum, int followerStmtNum) override;
    void insertFollows(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) override;
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
    void insertFollowsT(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) override;
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
    void insertParent(int parentStmtNum, const std::unordered_set<int>& childStmtSet) override;
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
    void insertUsesStmt(int stmtNum, std::string const & variable) override;
    void insertUsesStmt(int stmtNum, std::unordered_set<std::string> const & variableSet) override;
    std::vector<std::string> getUsesVariable(int stmtNum) override;
    std::vector<int> getUsesStmt(std::string const & variable) override;
    std::vector<int> getUsesStmt() override;
    std::vector<std::string> getUsesVariable() override;
    bool containsUsesStmt(int stmtNum) override;
    bool containsUsesVariable(std::string const & variable) override;
    bool containsUsesRelationship(int stmtNum, std::string const & variable) override;
    bool hasUsesRelationship() override;

    // Modifies Stmt Writer - Reader
    void insertModifiesStmt(int stmtNum, std::string const & variable) override;
    void insertModifiesStmt(int stmtNum, std::unordered_set<std::string> const & variableSet) override;
    std::vector<std::string> getModifiesVariable(int stmtNum) override;
    std::vector<int> getModifiesStmt(std::string const & variable) override;
    std::vector<int> getModifiesStmt() override;
    std::vector<std::string> getModifiesVariable() override;
    bool containsModifiesStmt(int stmtNum) override;
    bool containsModifiesVariable(std::string const & variable) override;
    bool containsModifiesRelationship(int stmtNum, std::string const & variable) override;
    bool hasModifiesRelationship() override;

    // Procedures
    // Uses Proc
    void insertUsesProc(std::string const & procedureName, std::string const & variable) override;
    void insertUsesProc(std::string const & procedureName, std::unordered_set<std::string> const & variableSet) override;
    std::vector<std::string> getUsesProcVariable(std::string const & procedureName) override;
    std::vector<std::string> getUsesProcVariable() override;
    std::vector<std::string> getUsesProcName() override;
    bool containsUsesProc(std::string const & procedureName) override;
    bool containsUsesProcVariable(std::string const & variable) override;
    bool containsUsesProcRelationship(std::string const & procedureName, std::string const & variable) override;
    bool hasUsesProcRelationship() override;

    // Modifies Proc
    void insertModifiesProc(std::string const & procedureName, std::string const & variable) override;
    void insertModifiesProc(std::string const & procedureName, std::unordered_set<std::string> const & variableSet) override;
    std::vector<std::string> getModifiesProcVariable(std::string const & procedureName) override;
    std::vector<std::string> getModifiesProcVariable() override;
    std::vector<std::string> getModifiesProcName() override;
    bool containsModifiesProc(std::string const & procedureName) override;
    bool containsModifiesProcVariable(std::string const & variable) override;
    bool containsModifiesProcRelationship(std::string const & procedureName, std::string const & variable) override;
    bool hasModifiesProcRelationship() override;


    // Assign Pattern
    void insertAssignPattern(std::string const & lhsVar, const size_t& rhsExprNodeHash, const int& stmtNum,
                             const std::shared_ptr<ExprNode>& nodePtr) override;

    std::vector<std::string> getAssignPatternLHS(size_t const & rhsExprNodeHash) override;
    std::vector<size_t> getAssignPatternRHS(std::string const & variable) override;
    std::vector<std::string> getAssignPatternLHS() override;
    std::vector<size_t> getAssignPatternRHS() override;

    // get stmt number
    std::vector<int> getAssignPatternStmtNum(std::string const & variable, size_t const & rhsExprNodeHash) override;
    std::vector<int> getAssignPatternLHSStmtNum(std::string const & variable) override;
    std::vector<int> getAssignPatternRHSStmtNum(size_t const & rhsExprNodeHash) override;

    std::vector<std::shared_ptr<ExprNode>> getAssignPatternRHSExprNodePtr(size_t const & rhsExprNodeHash) override;
    // get all assign pattern RHS node ptrs
    std::vector<std::shared_ptr<ExprNode>> getAssignPatternRHSExprNodePtr() override;

    bool containsAssignPattern(std::string const & variable, size_t const & rhsExprNodeHash) override;
    bool containsAssignPatternLHS(std::string const & variable) override;
    bool containsAssignPatternRHS(size_t const & rhsExprNodeHash) override;
    bool hasAssignPattern() override;

    // If Pattern
    void insertIfPattern(std::string const & variable, const int& stmtNum) override;
    void insertIfPattern(std::string const & variable, const std::unordered_set<int>& stmtSet) override;

    std::vector<std::string> getIfPatternVariable(int stmtNum) override;
    std::vector<int> getIfPatternStmtNum(std::string const & variable) override;
    std::vector<std::string> getIfPatternVariable() override;
    std::vector<int> getIfPatternStmtNum() override;
    bool containsIfPattern(std::string const & variable, int stmtNum) override;
    bool containsIfPatternStmt(int stmtNum) override;
    bool containsIfPatternVariable(std::string const & variable) override;
    bool hasIfPattern() override;

    // While Pattern
    void insertWhilePattern(std::string const & variable, const int& stmtNum) override;
    void insertWhilePattern(std::string const & variable, const std::unordered_set<int>& stmtSet) override;

    std::vector<std::string> getWhilePatternVariable(int stmtNum) override;
    std::vector<int> getWhilePatternStmtNum(std::string const & variable) override;
    std::vector<std::string> getWhilePatternVariable() override;
    std::vector<int> getWhilePatternStmtNum() override;
    bool containsWhilePattern(std::string const & variable, int stmtNum) override;
    bool containsWhilePatternStmt(int stmtNum) override;
    bool containsWhilePatternVariable(std::string const & variable) override;
    bool hasWhilePattern() override;

};
