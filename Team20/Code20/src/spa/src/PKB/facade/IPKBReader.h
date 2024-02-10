#pragma once

#include <unordered_set>
#include <string>

// interface for FacadeAPI to implement
class IPKBReader {

public:
    virtual ~IPKBReader() = default;

    // Procedure
    virtual std::unordered_set<std::string> getAllProcedures() = 0;
    virtual bool containsProcedure(std::string const & procedureName) = 0;

    // Variables
    virtual std::unordered_set<std::string> getAllVariables() = 0;
    virtual bool containsVariable(std::string const & variableName) = 0;

    // Constant
    virtual std::unordered_set<int> getAllConstants() = 0;
    virtual bool containsConstant(int constant) = 0;

    // Statement
    virtual std::unordered_set<int> getAllStatementNum() = 0;
    virtual bool containsStatement(int stmtNum) = 0;

    // Assign
    virtual std::unordered_set<int> getAllAssignStmtNum() = 0;
    virtual bool containsAssign(int stmtNum) = 0;

    // Read
    virtual std::unordered_set<int> getAllReadStmtNum() = 0;
    virtual bool containsRead(int stmtNum) = 0;

    // Print
    virtual std::unordered_set<int> getAllPrintStmtNum() = 0;
    virtual bool containsPrint(int stmtNum) = 0;

    // Call
    virtual std::unordered_set<int> getAllCallStmtNum() = 0;
    virtual bool containsCall(int stmtNum) = 0;

    // While
    virtual std::unordered_set<int> getAllWhileStmtNum() = 0;
    virtual bool containsWhile(int stmtNum) = 0;

    // If
    virtual std::unordered_set<int> getAllIfStmtNum() = 0;
    virtual bool containsIf(int stmtNum) = 0;

    // Relationship

    // Follows
    virtual std::unordered_set<int> getFollows(int followeeStmtNum) = 0;
    virtual std::unordered_set<int> getFollowing(int followerStmtNum) = 0;
    virtual bool containsFollows(int followeeStmtNum) = 0;
    virtual bool containsFollowing(int followerStmtNum) = 0;

    // FollowsT
    virtual std::unordered_set<int> getFollowsT(int followeeStmtNum) = 0;
    virtual std::unordered_set<int> getFollowingT(int followerStmtNum) = 0;
    virtual bool containsFollowsT(int followeeStmtNum) = 0;
    virtual bool containsFollowingT(int followerStmtNum) = 0;

    // Parent
    virtual std::unordered_set<int> getChild(int parentStmtNum) = 0;
    virtual std::unordered_set<int> getParent(int childStmtNum) = 0;
    virtual bool containsParent(int parentStmtNum) = 0;
    virtual bool containsChild(int childStmtNum) = 0;

    // ParentT
    virtual std::unordered_set<int> getChildT(int parentStmtNum) = 0;
    virtual std::unordered_set<int> getParentT(int childStmtNum) = 0;
    virtual bool containsParentT(int parentStmtNum) = 0;
    virtual bool containsChildT(int childStmtNum) = 0;
};