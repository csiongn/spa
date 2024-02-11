#pragma once

#include <vector>
#include <string>

// interface for FacadeAPI to implement
class IPKBReader {

public:
    virtual ~IPKBReader() = default;

    // Procedure
    virtual std::vector<std::string> getAllProcedures() = 0;
    virtual bool containsProcedure(std::string const & procedureName) = 0;

    // Variables
    virtual std::vector<std::string> getAllVariables() = 0;
    virtual bool containsVariable(std::string const & variableName) = 0;

    // Constant
    virtual std::vector<int> getAllConstants() = 0;
    virtual bool containsConstant(int constant) = 0;

    // Statement
    virtual std::vector<int> getAllStatementNum() = 0;
    virtual bool containsStatement(int stmtNum) = 0;

    // Assign
    virtual std::vector<int> getAllAssignStmtNum() = 0;
    virtual bool containsAssign(int stmtNum) = 0;

    // Read
    virtual std::vector<int> getAllReadStmtNum() = 0;
    virtual bool containsRead(int stmtNum) = 0;

    // Print
    virtual std::vector<int> getAllPrintStmtNum() = 0;
    virtual bool containsPrint(int stmtNum) = 0;

    // Call
    virtual std::vector<int> getAllCallStmtNum() = 0;
    virtual bool containsCall(int stmtNum) = 0;

    // While
    virtual std::vector<int> getAllWhileStmtNum() = 0;
    virtual bool containsWhile(int stmtNum) = 0;

    // If
    virtual std::vector<int> getAllIfStmtNum() = 0;
    virtual bool containsIf(int stmtNum) = 0;

    // Relationship

    // Follows
    virtual std::vector<int> getFollows(int followeeStmtNum) = 0;
    virtual std::vector<int> getFollowing(int followerStmtNum) = 0;
    virtual bool containsFollows(int followeeStmtNum) = 0;
    virtual bool containsFollowing(int followerStmtNum) = 0;

    // FollowsT
    virtual std::vector<int> getFollowsT(int followeeStmtNum) = 0;
    virtual std::vector<int> getFollowingT(int followerStmtNum) = 0;
    virtual bool containsFollowsT(int followeeStmtNum) = 0;
    virtual bool containsFollowingT(int followerStmtNum) = 0;

    // Parent
    virtual std::vector<int> getChild(int parentStmtNum) = 0;
    virtual std::vector<int> getParent(int childStmtNum) = 0;
    virtual bool containsParent(int parentStmtNum) = 0;
    virtual bool containsChild(int childStmtNum) = 0;

    // ParentT
    virtual std::vector<int> getChildT(int parentStmtNum) = 0;
    virtual std::vector<int> getParentT(int childStmtNum) = 0;
    virtual bool containsParentT(int parentStmtNum) = 0;
    virtual bool containsChildT(int childStmtNum) = 0;
};