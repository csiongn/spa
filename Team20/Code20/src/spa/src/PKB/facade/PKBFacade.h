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
    // No need to implement destructor as we are using smart pointers

    // add destructor for PKBFacade
    // implement the methods from IPKBWriter and IPKBReader

    // IPKBWriter
    // #followeeStmtNum
    // #followerStmtNum
    void insertFollows(int followeeStmtNum, int followerStmtNum) override;

    // IPKBReader
    // Follows
    std::vector<int> getFollows(int followeeStmtNum) override;
    std::vector<int> getFollowing(int followerStmtNum) override;
    bool containsFollows(int followeeStmtNum) override;
    bool containsFollowing(int followerStmtNum) override;
};
