#include <utility>
#include <vector>
#include <string>

#include "PKBFacade.h"
#include "PKB/Database.h"

// What is the std::move warning
PKBFacade::PKBFacade(std::shared_ptr<Database> db) : db(std::move(db)) {}

// SP
void PKBFacade::insertFollows(int followeeStmtNum, int followerStmtNum) {
    db->getFollowsManager()->insert(followeeStmtNum, followerStmtNum);
}

// QPS
bool PKBFacade::containsFollowing(int followerStmtNum) {
    return db->getFollowsManager()->containsReverse(followerStmtNum);
}

bool PKBFacade::containsFollows(int followeeStmtNum) {
    return db->getFollowsManager()->contains(followeeStmtNum);
}

std::vector<int> PKBFacade::getFollowing(int followerStmtNum) {
    return db->getFollowsManager()->getReverse(followerStmtNum);
}

std::vector<int> PKBFacade::getFollows(int followeeStmtNum) {
    return db->getFollowsManager()->get(followeeStmtNum);
};



