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
    std::vector<std::string> getAllProcedures() override;
    bool containsProcedure(std::string const & procedure) override;

    // Variable
    void insertVariable(std::string const & variable) override;
    std::vector<std::string> getAllVariables() override;
    bool containsVariable(std::string const & variable) override;

    // Constant
    void insertConstant(int constant) override;
    std::vector<int> getAllConstants() override;
    bool containsConstant(int constant) override;

    // Statement
    void insertStatement(int stmtNum) override;
    std::vector<int> getAllStatementNum() override;
    bool containsStatement(int stmtNum) override;


    // Assign
    void insertAssign(int stmtNum) override;
    std::vector<int> getAllAssignStmtNum() override;
    bool containsAssign(int stmtNum) override;

    // Read
    void insertRead(int stmtNum) override;
    std::vector<int> getAllReadStmtNum() override;
    bool containsRead(int stmtNum) override;

    // Print
    void insertPrint(int stmtNum) override;
    std::vector<int> getAllPrintStmtNum() override;
    bool containsPrint(int stmtNum) override;

    // Call
    void insertCall(int stmtNum) override;
    std::vector<int> getAllCallStmtNum() override;
    bool containsCall(int stmtNum) override;

    // While
    void insertWhile(int stmtNum) override;
    std::vector<int> getAllWhileStmtNum() override;
    bool containsWhile(int stmtNum) override;

    // If
    void insertIf(int stmtNum) override;
    std::vector<int> getAllIfStmtNum() override;
    bool containsIf(int stmtNum) override;

    // Follows Writer - Reader
    void insertFollows(int followeeStmtNum, int followerStmtNum) override;
    std::vector<int> getFollows(int followeeStmtNum) override;
    std::vector<int> getFollowing(int followerStmtNum) override;
    bool containsFollows(int followeeStmtNum) override;
    bool containsFollowing(int followerStmtNum) override;

    // FollowsT Writer - Reader
    void insertFollowsT(int followeeStmtNum, int followerStmtNum) override;
    std::vector<int> getFollowsT(int followeeStmtNum) override;
    std::vector<int> getFollowingT(int followerStmtNum) override;
    bool containsFollowsT(int followeeStmtNum) override;
    bool containsFollowingT(int followerStmtNum) override;

    // Parent Writer - Reader
    void insertParent(int parentStmtNum, int childStmtNum) override;
    std::vector<int> getChild(int parentStmtNum) override;
    std::vector<int> getParent(int childStmtNum) override;
    bool containsParent(int parentStmtNum) override;
    bool containsChild(int childStmtNum) override;

    // ParentT Writer - Reader
    void insertParentT(int parentStmtNum, int childStmtNum) override;
    std::vector<int> getChildT(int parentStmtNum) override;
    std::vector<int> getParentT(int childStmtNum) override;
    bool containsParentT(int parentStmtNum) override;
    bool containsChildT(int childStmtNum) override;
};
