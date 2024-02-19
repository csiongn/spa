#include <utility>
#include <vector>
#include <string>
#include <iostream>

#include "PKBFacade.h"
#include "PKB/Database.h"

// What is the std::move warning
PKBFacade::PKBFacade(std::shared_ptr<Database> db) : db(std::move(db)) {}

// Procedure
void PKBFacade::insertProcedure(std::string const & procedure) {
    db->getProcedureManager()->insert(procedure);
}
void PKBFacade::insertProcedure(const std::unordered_set<std::string>& procedureSet) {
    for (const auto &procedure : procedureSet) {
        insertProcedure(procedure);
    }
}

std::vector<std::string> PKBFacade::getAllProcedures() {
    return db->getProcedureManager()->get();
}

bool PKBFacade::containsProcedure(std::string const & procedure) {
    return db->getProcedureManager()->contains(procedure);
}

// Variable
void PKBFacade::insertVariable(std::string const & variable) {
    db->getVariableManager()->insert(variable);
};

void PKBFacade::insertVariable(const std::unordered_set<std::string>& variableSet) {
    for (const auto &variable : variableSet) {
        insertVariable(variable);
    }
}

bool PKBFacade::containsVariable(std::string const & variable) {
    return db->getVariableManager()->contains(variable);
}

std::vector<std::string> PKBFacade::getAllVariables() {
    return db->getVariableManager()->get();
}

// Constant
void PKBFacade::insertConstant(int constant) {
    db->getConstantManager()->insert(constant);
}

void PKBFacade::insertConstant(const std::unordered_set<int>& constantSet) {
    for (const auto &constant : constantSet) {
        insertConstant(constant);
    }
}

std::vector<int> PKBFacade::getAllConstants() {
    return db->getConstantManager()->get();
}

bool PKBFacade::containsConstant(int constant) {
    return db->getConstantManager()->contains(constant);
}

// Statement
void PKBFacade::insertStatement(int stmtNum) {
    db->getStatementManager()->insert(stmtNum);
}

void PKBFacade::insertStatement(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        insertStatement(stmt);
    }
}

std::vector<int> PKBFacade::getAllStatementNum() {
    return db->getStatementManager()->get();
}

bool PKBFacade::containsStatement(int stmtNum) {
    return db->getStatementManager()->contains(stmtNum);
}

// Assign
void PKBFacade::insertAssign(int stmtNum) {
    db->getAssignManager()->insert(stmtNum);
}

void PKBFacade::insertAssign(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        insertAssign(stmt);
    }
}

std::vector<int> PKBFacade::getAllAssignStmtNum() {
    return db->getAssignManager()->get();
}

bool PKBFacade::containsAssign(int stmtNum) {
    return db->getAssignManager()->contains(stmtNum);
}

// Read
void PKBFacade::insertRead(int stmtNum) {
    db->getReadManager()->insert(stmtNum);
}

void PKBFacade::insertRead(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        insertRead(stmt);
    }
}

std::vector<int> PKBFacade::getAllReadStmtNum() {
    return db->getReadManager()->get();
}

bool PKBFacade::containsRead(int stmtNum) {
    return db->getReadManager()->contains(stmtNum);
}

// Print
void PKBFacade::insertPrint(int stmtNum) {
    db->getPrintManager()->insert(stmtNum);
}

void PKBFacade::insertPrint(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        insertPrint(stmt);
    }
}

std::vector<int> PKBFacade::getAllPrintStmtNum() {
    return db->getPrintManager()->get();
}

bool PKBFacade::containsPrint(int stmtNum) {
    return db->getPrintManager()->contains(stmtNum);
}

// Call
void PKBFacade::insertCall(int stmtNum) {
    db->getCallManager()->insert(stmtNum);
}

void PKBFacade::insertCall(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        insertCall(stmt);
    }
}

std::vector<int> PKBFacade::getAllCallStmtNum() {
    return db->getCallManager()->get();
}

bool PKBFacade::containsCall(int stmtNum) {
    return db->getCallManager()->contains(stmtNum);
}

// While
void PKBFacade::insertWhile(int stmtNum) {
    db->getWhileManager()->insert(stmtNum);
}

void PKBFacade::insertWhile(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        insertWhile(stmt);
    }
}

std::vector<int> PKBFacade::getAllWhileStmtNum() {
    return db->getWhileManager()->get();
}

bool PKBFacade::containsWhile(int stmtNum) {
    return db->getWhileManager()->contains(stmtNum);
}

// If
void PKBFacade::insertIf(int stmtNum) {
    db->getIfManager()->insert(stmtNum);
}

void PKBFacade::insertIf(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        insertIf(stmt);
    }
}

std::vector<int> PKBFacade::getAllIfStmtNum() {
    return db->getIfManager()->get();
}

bool PKBFacade::containsIf(int stmtNum) {
    return db->getIfManager()->contains(stmtNum);
}

// Follows
void PKBFacade::insertFollows(int followeeStmtNum, int followerStmtNum) {
    db->getFollowsManager()->insert(followeeStmtNum, followerStmtNum);
}

void PKBFacade::insertFollows(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) {
    for (const auto &follower : followerStmtSet) {
        insertFollows(followeeStmtNum, follower);
    }
}

bool PKBFacade::containsFollows(int followeeStmtNum) {
    return db->getFollowsManager()->contains(followeeStmtNum);
}


bool PKBFacade::containsFollowing(int followerStmtNum) {
    return db->getFollowsManager()->containsReverse(followerStmtNum);
}

bool PKBFacade::containsFollowsRelationship(int followeeStmtNum, int followerStmtNum) {
    return db->getFollowsManager()->containsValueInKeySet(followeeStmtNum, followerStmtNum);
}

bool PKBFacade::hasFollowsRelationship() {
    return db->getFollowsManager()->hasRelationship();
}

std::vector<int> PKBFacade::getFollows(int followeeStmtNum) {
    return db->getFollowsManager()->get(followeeStmtNum);
}

std::vector<int> PKBFacade::getFollowing(int followerStmtNum) {
    return db->getFollowsManager()->getReverse(followerStmtNum);
}

// FollowsT
void PKBFacade::insertFollowsT(int followeeStmtNum, int followerStmtNum) {
    db->getFollowsTManager()->insert(followeeStmtNum, followerStmtNum);
}

void PKBFacade::insertFollowsT(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) {
    for (const auto &follower : followerStmtSet) {
        insertFollowsT(followeeStmtNum, follower);
    }
}

bool PKBFacade::containsFollowsT(int followeeStmtNum) {
    return db->getFollowsTManager()->contains(followeeStmtNum);
}

bool PKBFacade::containsFollowingT(int followerStmtNum) {
    return db->getFollowsTManager()->containsReverse(followerStmtNum);
}

bool PKBFacade::containsFollowsTRelationship(int followeeStmtNum, int followerStmtNum) {
    return db->getFollowsTManager()->containsValueInKeySet(followeeStmtNum, followerStmtNum);
}

bool PKBFacade::hasFollowsTRelationship() {
    return db->getFollowsTManager()->hasRelationship();
}

std::vector<int> PKBFacade::getFollowsT(int followeeStmtNum) {
    return db->getFollowsTManager()->get(followeeStmtNum);
}

std::vector<int> PKBFacade::getFollowingT(int followerStmtNum) {
    return db->getFollowsTManager()->getReverse(followerStmtNum);
}

// Parent
void PKBFacade::insertParent(int parentStmtNum, int childStmtNum) {
    db->getParentManager()->insert(parentStmtNum, childStmtNum);
}

void PKBFacade::insertParent(int parentStmtNum, const std::unordered_set<int>& childStmtSet) {
    for (const auto &child : childStmtSet) {
        insertParent(parentStmtNum, child);
    }
}

bool PKBFacade::containsParent(int parentStmtNum) {
    return db->getParentManager()->contains(parentStmtNum);
}

bool PKBFacade::containsChild(int childStmtNum) {
    return db->getParentManager()->containsReverse(childStmtNum);
}

bool PKBFacade::containsParentRelationship(int parentStmtNum, int childStmtNum) {
    return db->getParentManager()->containsValueInKeySet(parentStmtNum, childStmtNum);
}

bool PKBFacade::hasParentRelationship() {
    return db->getParentManager()->hasRelationship();
}

// Parent: set(child) : data
std::vector<int> PKBFacade::getChild(int parentStmtNum) {
    return db->getParentManager()->get(parentStmtNum);
}

// Child: set(parent) : reverseData
std::vector<int> PKBFacade::getParent(int childStmtNum) {
    return db->getParentManager()->getReverse(childStmtNum);
}

// ParentT
void PKBFacade::insertParentT(int parentStmtNum, int childStmtNum) {
    db->getParentTManager()->insert(parentStmtNum, childStmtNum);
}

void PKBFacade::insertParentT(int parentStmtNum, const std::unordered_set<int>& childStmtSet) {
    for (const auto &child : childStmtSet) {
        insertParentT(parentStmtNum, child);
    }
}

bool PKBFacade::containsParentT(int parentStmtNum) {
    return db->getParentTManager()->contains(parentStmtNum);
}

bool PKBFacade::containsChildT(int childStmtNum) {
    return db->getParentTManager()->containsReverse(childStmtNum);
}

bool PKBFacade::containsParentTRelationship(int parentStmtNum, int childStmtNum) {
    return db->getParentTManager()->containsValueInKeySet(parentStmtNum, childStmtNum);
}

bool PKBFacade::hasParentTRelationship() {
    return db->getParentTManager()->hasRelationship();
}

// Parent: set(child) : data
std::vector<int> PKBFacade::getChildT(int parentStmtNum) {
    return db->getParentTManager()->get(parentStmtNum);
}

// Child: set(parent) : reverseData
std::vector<int> PKBFacade::getParentT(int childStmtNum) {
    return db->getParentTManager()->getReverse(childStmtNum);
}

// Uses
void PKBFacade::insertUsesStmt(int stmtNum, std::string const & variable) {
    db->getUsesStmtManager()->insert(stmtNum, variable);
}

void PKBFacade::insertUsesStmt(int stmtNum, const std::unordered_set<std::string>& variableSet) {
    for (const auto &variable: variableSet) {
        insertUsesStmt(stmtNum, variable);
    }
}

std::vector<std::string> PKBFacade::getUsesVariable(int stmtNum) {
    return db->getUsesStmtManager()->get(stmtNum);
}

std::vector<int> PKBFacade::getUsesStmt(std::string const & variable) {
    return db->getUsesStmtManager()->getReverse(variable);
}

bool PKBFacade::containsUsesStmt(int stmtNum) {
    return db->getUsesStmtManager()->contains(stmtNum);
}

bool PKBFacade::containsUsesVariable(std::string const & variable) {
    return db->getUsesStmtManager()->containsReverse(variable);
}

bool PKBFacade::containsUsesRelationship(int stmtNum, const std::string &variable) {
    return db->getUsesStmtManager()->containsValueInKeySet(stmtNum, variable);
}

bool PKBFacade::hasUsesRelationship() {
    return db->getUsesStmtManager()->hasRelationship();
}

// Modifies
void PKBFacade::insertModifiesStmt(int stmtNum, const std::string &variable) {
    db->getModifiesStmtManager()->insert(stmtNum, variable);
}

void PKBFacade::insertModifiesStmt(int stmtNum, const std::unordered_set<std::string>& variableSet) {
    for (const auto &variable: variableSet) {
        insertModifiesStmt(stmtNum, variable);
    }
}

std::vector<std::string> PKBFacade::getModifiesVariable(int stmtNum) {
    return db->getModifiesStmtManager()->get(stmtNum);
}

std::vector<int> PKBFacade::getModifiesStmt(std::string const & variable) {
    return db->getModifiesStmtManager()->getReverse(variable);
}

bool PKBFacade::containsModifiesStmt(int stmtNum) {
    return db->getModifiesStmtManager()->contains(stmtNum);
}

bool PKBFacade::containsModifiesVariable(std::string const & variable) {
    return db->getModifiesStmtManager()->containsReverse(variable);
}

bool PKBFacade::containsModifiesRelationship(int stmtNum, std::string const & variable) {
    return db->getModifiesStmtManager()->containsValueInKeySet(stmtNum, variable);
}

bool PKBFacade::hasModifiesRelationship() {
    return db->getModifiesStmtManager()->hasRelationship();
}