#pragma once

class IPKBWriter {

public:
    virtual ~IPKBWriter() = default;

    // Procedure
    virtual void insertProcedure(std::string const & procedureName) = 0;

    // Variable
    virtual void insertVariable(std::string const & variableName) = 0;

    // Constant
    virtual void insertConstant(int constant) = 0;

    // Statement
    virtual void insertStatement(int stmtNum) = 0;

    // Assign
    virtual void insertAssign(int stmtNum) = 0;

    // Read
    virtual void insertRead(int stmtNum) = 0;

    // Print
    virtual void insertPrint(int stmtNum) = 0;

    // Call
    virtual void insertCall(int stmtNum) = 0;

    // While
    virtual void insertWhile(int stmtNum) = 0;

    // If
    virtual void insertIf(int stmtNum) = 0;

    // Follows
    virtual void insertFollows(int followeeStmtNum, int followerStmtNum) = 0;

    // FollowsT
    virtual void insertFollowsT(int followeeStmtNum, int followerStmtNum) = 0;

    // Parent
    virtual void insertParent(int parentStmtNum, int childStmtNum) = 0;

    // ParentT
    virtual void insertParentT(int parentStmtNum, int childStmtNum) = 0;
};