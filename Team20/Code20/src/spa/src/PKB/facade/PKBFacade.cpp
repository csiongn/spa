#include <utility>
#include <unordered_set>
#include <string>

#include "PKBFacade.h"
#include "PKB/Database.h"

// What is the std::move warning
PKBFacade::PKBFacade(std::shared_ptr<Database> db) : db(std::move(db)) {}

// Variables
void PKBFacade::insertVariable(std::string const & variable) {
    db->getVariableManager()->insert(variable);
};

bool PKBFacade::containsVariable(std::string const & variable) {
    return db->getVariableManager()->contains(variable);
}

std::unordered_set<std::string> PKBFacade::getAllVariables() {
    return db->getVariableManager()->get();
}

// Assign
void PKBFacade::insertAssign(int stmtNum) {
    db->getAssignManager()->insert(stmtNum);
}

std::unordered_set<int> PKBFacade::getAllAssignStmtNum() {
    return db->getAssignManager()->get();
}


// Follows
void PKBFacade::insertFollows(int followeeStmtNum, int followerStmtNum) {
    db->getFollowsManager()->insert(followeeStmtNum, followerStmtNum);
}

bool PKBFacade::containsFollows(int followeeStmtNum) {
    return db->getFollowsManager()->contains(followeeStmtNum);
}


bool PKBFacade::containsFollowing(int followerStmtNum) {
    return db->getFollowsManager()->containsReverse(followerStmtNum);
}

std::unordered_set<int> PKBFacade::getFollows(int followeeStmtNum) {
    return db->getFollowsManager()->get(followeeStmtNum);
}

std::unordered_set<int> PKBFacade::getFollowing(int followerStmtNum) {
    return db->getFollowsManager()->getReverse(followerStmtNum);
}

// FollowsT
void PKBFacade::insertFollowsT(int followeeStmtNum, int followerStmtNum) {
    db->getFollowsTManager()->insert(followeeStmtNum, followerStmtNum);
}

bool PKBFacade::containsFollowsT(int followeeStmtNum) {
    return db->getFollowsTManager()->contains(followeeStmtNum);
}

bool PKBFacade::containsFollowingT(int followerStmtNum) {
    return db->getFollowsTManager()->containsReverse(followerStmtNum);
}

std::unordered_set<int> PKBFacade::getFollowsT(int followeeStmtNum) {
    return db->getFollowsTManager()->get(followeeStmtNum);
}

std::unordered_set<int> PKBFacade::getFollowingT(int followerStmtNum) {
    return db->getFollowsTManager()->getReverse(followerStmtNum);
}

// Parent
void PKBFacade::insertParent(int parentStmtNum, int childStmtNum) {
    db->getParentManager()->insert(parentStmtNum, childStmtNum);
}

bool PKBFacade::containsParent(int parentStmtNum) {
    return db->getParentManager()->contains(parentStmtNum);
}

bool PKBFacade::containsChild(int childStmtNum) {
    return db->getParentManager()->containsReverse(childStmtNum);
}

// Parent: set(child) : data
std::unordered_set<int> PKBFacade::getChild(int parentStmtNum) {
    return db->getParentManager()->get(parentStmtNum);
}

// Child: set(parent) : reverseData
std::unordered_set<int> PKBFacade::getParent(int childStmtNum) {
    return db->getParentManager()->getReverse(childStmtNum);
}

// ParentT
void PKBFacade::insertParentT(int parentStmtNum, int childStmtNum) {
    db->getParentTManager()->insert(parentStmtNum, childStmtNum);
}

bool PKBFacade::containsParentT(int parentStmtNum) {
    return db->getParentTManager()->contains(parentStmtNum);
}

bool PKBFacade::containsChildT(int childStmtNum) {
    return db->getParentTManager()->containsReverse(childStmtNum);
}

// Parent: set(child) : data
std::unordered_set<int> PKBFacade::getChildT(int parentStmtNum) {
    return db->getParentTManager()->get(parentStmtNum);
}

// Child: set(parent) : reverseData
std::unordered_set<int> PKBFacade::getParentT(int childStmtNum) {
    return db->getParentTManager()->getReverse(childStmtNum);
}
