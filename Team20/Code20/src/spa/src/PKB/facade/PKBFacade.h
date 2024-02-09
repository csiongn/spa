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

    // Variable
    void insertVariable(std::string const & variable) override;
    std::unordered_set<std::string> getAllVariables() override;
    bool containsVariable(std::string const & variable) override;

    // Assign
    void insertAssign(int stmtNum) override;
    std::unordered_set<int> getAllAssignStmtNum() override;

    // Follows Writer - Reader
    void insertFollows(int followeeStmtNum, int followerStmtNum) override;
    std::unordered_set<int> getFollows(int followeeStmtNum) override;
    std::unordered_set<int> getFollowing(int followerStmtNum) override;
    bool containsFollows(int followeeStmtNum) override;
    bool containsFollowing(int followerStmtNum) override;

    // FollowsT Writer - Reader
    void insertFollowsT(int followeeStmtNum, int followerStmtNum) override;
    std::unordered_set<int> getFollowsT(int followeeStmtNum) override;
    std::unordered_set<int> getFollowingT(int followerStmtNum) override;
    bool containsFollowsT(int followeeStmtNum) override;
    bool containsFollowingT(int followerStmtNum) override;

    // Parent Writer - Reader
    void insertParent(int parentStmtNum, int childStmtNum) override;
    std::unordered_set<int> getChild(int parentStmtNum) override;
    std::unordered_set<int> getParent(int childStmtNum) override;
    bool containsParent(int parentStmtNum) override;
    bool containsChild(int childStmtNum) override;

    // ParentT Writer - Reader
    void insertParentT(int parentStmtNum, int childStmtNum) override;
    std::unordered_set<int> getChildT(int parentStmtNum) override;
    std::unordered_set<int> getParentT(int childStmtNum) override;
    bool containsParentT(int parentStmtNum) override;
    bool containsChildT(int childStmtNum) override;
};
