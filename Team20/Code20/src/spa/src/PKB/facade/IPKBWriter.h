#pragma once

class IPKBWriter {

public:
    virtual ~IPKBWriter() = default;

    // Variables
    virtual void insertVariable(std::string const & variableName) = 0;

    // Assign
    virtual void insertAssign(int stmtNum) = 0;

    // Follows
    virtual void insertFollows(int followeeStmtNum, int followerStmtNum) = 0;

    // FollowsT
    virtual void insertFollowsT(int followeeStmtNum, int followerStmtNum) = 0;

    // Parent
    virtual void insertParent(int parentStmtNum, int childStmtNum) = 0;

    // ParentT
    virtual void insertParentT(int parentStmtNum, int childStmtNum) = 0;
};