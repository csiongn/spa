#pragma once

#include "unordered_set"

class IPKBWriter {

public:
    virtual ~IPKBWriter() = default;

    // Procedure
    virtual void insertProcedure(std::string const & procedureName) = 0;
    // Support set operations
    virtual void insertProcedure(const std::unordered_set<std::string>& procedureSet) = 0;

    // Variable
    virtual void insertVariable(std::string const & variableName) = 0;
    // Support set operations
    virtual void insertVariable(const std::unordered_set<std::string>& variableSet) = 0;

    // Constant
    virtual void insertConstant(int constant) = 0;
    // Support set operations
    virtual void insertConstant(const std::unordered_set<int>& constantSet) = 0;

    // Statement
    virtual void insertStatement(int stmtNum) = 0;
    // Support set operations
    virtual void insertStatement(const std::unordered_set<int>& stmtSet) = 0;

    // Assign
    virtual void insertAssign(int stmtNum) = 0;
    // Support set operations
    virtual void insertAssign(const std::unordered_set<int>& stmtSet) = 0;

    // Read
    virtual void insertRead(int stmtNum) = 0;
    // Support set operations
    virtual void insertRead(const std::unordered_set<int>& stmtSet) = 0;

    // Print
    virtual void insertPrint(int stmtNum) = 0;
    // Support set operations
    virtual void insertPrint(const std::unordered_set<int>& stmtSet) = 0;

    // Call
    virtual void insertCall(int stmtNum) = 0;
    // Support set operations
    virtual void insertCall(const std::unordered_set<int>& stmtSet) = 0;
    // While
    virtual void insertWhile(int stmtNum) = 0;
    // Support set operations
    virtual void insertWhile(const std::unordered_set<int>& stmtSet) = 0;

    // If
    virtual void insertIf(int stmtNum) = 0;
    // Support set operations
    virtual void insertIf(const std::unordered_set<int>& stmtSet) = 0;

    // Follows
    virtual void insertFollows(int followeeStmtNum, int followerStmtNum) = 0;
    // Support set operations
    virtual void insertFollows(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) = 0;

    // FollowsT
    virtual void insertFollowsT(int followeeStmtNum, int followerStmtNum) = 0;
    // Support set operations
    virtual void insertFollowsT(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) = 0;

    // Parent
    virtual void insertParent(int parentStmtNum, int childStmtNum) = 0;
    // Support set operations
    virtual void insertParent(int parentStmtNum, const std::unordered_set<int>& childStmtSet) = 0;

    // ParentT
    virtual void insertParentT(int parentStmtNum, int childStmtNum) = 0;
    // Support set operations
    virtual void insertParentT(int parentStmtNum, const std::unordered_set<int>& childStmtSet) = 0;

    // Uses
    virtual void insertUsesStmt(int stmtNum, std::string const & variable) = 0;
    virtual void insertUsesStmt(int stmtNum, std::unordered_set<std::string> const & variableSet) = 0;

    // Modifies
    virtual void insertModifiesStmt(int stmtNum, std::string const & variable) = 0;
    virtual void insertModifiesStmt(int stmtNum, std::unordered_set<std::string> const & variableSet) = 0;

};