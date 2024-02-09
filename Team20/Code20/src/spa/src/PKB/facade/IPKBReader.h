#pragma once

#include <unordered_set>
#include <string>

// interface for FacadeAPI to implement
class IPKBReader {

public:
    virtual ~IPKBReader() = default;

    // Variables
    virtual std::unordered_set<std::string> getAllVariables() = 0;
    virtual bool containsVariable(std::string const & variableName) = 0;

    // Assign
    virtual std::unordered_set<int> getAllAssignStmtNum() = 0;

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