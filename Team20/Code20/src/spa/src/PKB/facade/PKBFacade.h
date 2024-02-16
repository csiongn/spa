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
    bool containsFollows(int followeeStmtNum) override;
    bool containsFollowing(int followerStmtNum) override;

    // FollowsT Writer - Reader
    void insertFollowsT(int followeeStmtNum, int followerStmtNum) override;
    void insertFollowsT(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) override;
    std::vector<int> getFollowsT(int followeeStmtNum) override;
    std::vector<int> getFollowingT(int followerStmtNum) override;
    bool containsFollowsT(int followeeStmtNum) override;
    bool containsFollowingT(int followerStmtNum) override;

    // Parent Writer - Reader
    void insertParent(int parentStmtNum, int childStmtNum) override;
    void insertParent(int parentStmtNum, const std::unordered_set<int>& childStmtSet) override;
    std::vector<int> getChild(int parentStmtNum) override;
    std::vector<int> getParent(int childStmtNum) override;
    bool containsParent(int parentStmtNum) override;
    bool containsChild(int childStmtNum) override;

    // ParentT Writer - Reader
    void insertParentT(int parentStmtNum, int childStmtNum) override;
    void insertParentT(int parentStmtNum, const std::unordered_set<int> &childStmtSet) override;
    std::vector<int> getChildT(int parentStmtNum) override;
    std::vector<int> getParentT(int childStmtNum) override;
    bool containsParentT(int parentStmtNum) override;
    bool containsChildT(int childStmtNum) override;
};
