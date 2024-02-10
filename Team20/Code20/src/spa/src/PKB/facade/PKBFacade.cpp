#include <utility>
#include <unordered_set>
#include <string>

#include "PKBFacade.h"
#include "PKB/Database.h"

// What is the std::move warning
PKBFacade::PKBFacade(std::shared_ptr<Database> db) : db(std::move(db)) {}

// Procedure
void PKBFacade::insertProcedure(std::string const & procedure) {
    db->getProcedureManager()->insert(procedure);
}

std::unordered_set<std::string> PKBFacade::getAllProcedures() {
    return db->getProcedureManager()->get();
}

bool PKBFacade::containsProcedure(std::string const & procedure) {
    return db->getProcedureManager()->contains(procedure);
}

// Variable
void PKBFacade::insertVariable(std::string const & variable) {
    db->getVariableManager()->insert(variable);
};

bool PKBFacade::containsVariable(std::string const & variable) {
    return db->getVariableManager()->contains(variable);
}

std::unordered_set<std::string> PKBFacade::getAllVariables() {
    return db->getVariableManager()->get();
}

// Constant
void PKBFacade::insertConstant(int constant) {
    db->getConstantManager()->insert(constant);
}

std::unordered_set<int> PKBFacade::getAllConstants() {
    return db->getConstantManager()->get();
}

bool PKBFacade::containsConstant(int constant) {
    return db->getConstantManager()->contains(constant);
}

// Statement
void PKBFacade::insertStatement(int stmtNum) {
    db->getStatementManager()->insert(stmtNum);
}

std::unordered_set<int> PKBFacade::getAllStatementNum() {
    return db->getStatementManager()->get();
}

bool PKBFacade::containsStatement(int stmtNum) {
    return db->getStatementManager()->contains(stmtNum);
}

// Assign
void PKBFacade::insertAssign(int stmtNum) {
    db->getAssignManager()->insert(stmtNum);
}

std::unordered_set<int> PKBFacade::getAllAssignStmtNum() {
    return db->getAssignManager()->get();
}

bool PKBFacade::containsAssign(int stmtNum) {
    return db->getAssignManager()->contains(stmtNum);
}

// Read
void PKBFacade::insertRead(int stmtNum) {
    db->getReadManager()->insert(stmtNum);
}

std::unordered_set<int> PKBFacade::getAllReadStmtNum() {
    return db->getReadManager()->get();
}

bool PKBFacade::containsRead(int stmtNum) {
    return db->getReadManager()->contains(stmtNum);
}

// Print
void PKBFacade::insertPrint(int stmtNum) {
    db->getPrintManager()->insert(stmtNum);
}

std::unordered_set<int> PKBFacade::getAllPrintStmtNum() {
    return db->getPrintManager()->get();
}

bool PKBFacade::containsPrint(int stmtNum) {
    return db->getPrintManager()->contains(stmtNum);
}

// Call
void PKBFacade::insertCall(int stmtNum) {
    db->getCallManager()->insert(stmtNum);
}

std::unordered_set<int> PKBFacade::getAllCallStmtNum() {
    return db->getCallManager()->get();
}

bool PKBFacade::containsCall(int stmtNum) {
    return db->getCallManager()->contains(stmtNum);
}

// While
void PKBFacade::insertWhile(int stmtNum) {
    db->getWhileManager()->insert(stmtNum);
}

std::unordered_set<int> PKBFacade::getAllWhileStmtNum() {
    return db->getWhileManager()->get();
}

bool PKBFacade::containsWhile(int stmtNum) {
    return db->getWhileManager()->contains(stmtNum);
}

// If
void PKBFacade::insertIf(int stmtNum) {
    db->getIfManager()->insert(stmtNum);
}

std::unordered_set<int> PKBFacade::getAllIfStmtNum() {
    return db->getIfManager()->get();
}

bool PKBFacade::containsIf(int stmtNum) {
    return db->getIfManager()->contains(stmtNum);
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
