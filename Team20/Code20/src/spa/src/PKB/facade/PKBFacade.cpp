#include <utility>
#include <vector>
#include <string>

#include "PKBFacade.h"
#include "PKB/Database.h"
#include "PKB/utils/DatabaseUtils.h"

PKBFacade::PKBFacade(std::shared_ptr<Database> db) : db(std::move(db)) {}

// Procedure
void PKBFacade::insertProcedure(std::string const & procedure) {
    DatabaseUtils::insertEntity<std::string>(db->getProcedureManager(), procedure);
}
void PKBFacade::insertProcedure(const std::unordered_set<std::string>& procedureSet) {
    for (const auto &procedure : procedureSet) {
        DatabaseUtils::insertEntity<std::string>(db->getProcedureManager(), procedure);
    }
}

std::vector<std::string> PKBFacade::getAllProcedures() {
    return DatabaseUtils::getAllEntities<std::string>(db->getProcedureManager());
}

bool PKBFacade::containsProcedure(std::string const & procedure) {
    return DatabaseUtils::containsEntity<std::string>(db->getProcedureManager(), procedure);
}

bool PKBFacade::hasProcedure() {
    return DatabaseUtils::hasEntity<std::string>(db->getProcedureManager());
}

// Variable
void PKBFacade::insertVariable(std::string const & variable) {
    DatabaseUtils::insertEntity<std::string>(db->getVariableManager(), variable);
};

void PKBFacade::insertVariable(const std::unordered_set<std::string>& variableSet) {
    for (const auto &variable : variableSet) {
        DatabaseUtils::insertEntity<std::string>(db->getVariableManager(), variable);
    }
}

std::vector<std::string> PKBFacade::getAllVariables() {
    return DatabaseUtils::getAllEntities<std::string>(db->getVariableManager());
}

bool PKBFacade::containsVariable(std::string const & variable) {
    return DatabaseUtils::containsEntity<std::string>(db->getVariableManager(), variable);
}

bool PKBFacade::hasVariable() {
    return DatabaseUtils::hasEntity<std::string>(db->getVariableManager());
}

// Constant
void PKBFacade::insertConstant(int constant) {
    DatabaseUtils::insertEntity<int>(db->getConstantManager(), constant);
}

void PKBFacade::insertConstant(const std::unordered_set<int>& constantSet) {
    for (const auto &constant : constantSet) {
        DatabaseUtils::insertEntity<int>(db->getConstantManager(), constant);
    }
}

std::vector<int> PKBFacade::getAllConstants() {
    return DatabaseUtils::getAllEntities<int>(db->getConstantManager());
}

bool PKBFacade::containsConstant(int constant) {
    return DatabaseUtils::containsEntity<int>(db->getConstantManager(), constant);
}

bool PKBFacade::hasConstant() {
    return DatabaseUtils::hasEntity<int>(db->getConstantManager());
}

// Statement
void PKBFacade::insertStatement(int stmtNum) {
    DatabaseUtils::insertEntity<int>(db->getStatementManager(), stmtNum);
}

void PKBFacade::insertStatement(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertEntity<int>(db->getStatementManager(), stmt);
    }
}

std::vector<int> PKBFacade::getAllStatementNum() {
    return DatabaseUtils::getAllEntities<int>(db->getStatementManager());
}

bool PKBFacade::containsStatement(int stmtNum) {
    return DatabaseUtils::containsEntity<int>(db->getStatementManager(), stmtNum);
}

bool PKBFacade::hasStatement() {
    return DatabaseUtils::hasEntity<int>(db->getStatementManager());
}

// Assign
void PKBFacade::insertAssign(int stmtNum) {
    DatabaseUtils::insertEntity<int>(db->getAssignManager(), stmtNum);
}

void PKBFacade::insertAssign(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertEntity<int>(db->getAssignManager(), stmt);
    }
}

std::vector<int> PKBFacade::getAllAssignStmtNum() {
    return DatabaseUtils::getAllEntities<int>(db->getAssignManager());
}

bool PKBFacade::containsAssign(int stmtNum) {
    return DatabaseUtils::containsEntity<int>(db->getAssignManager(), stmtNum);
}

bool PKBFacade::hasAssign() {
    return DatabaseUtils::hasEntity<int>(db->getAssignManager());
}

// Read
void PKBFacade::insertRead(int stmtNum) {
    DatabaseUtils::insertEntity<int>(db->getReadManager(), stmtNum);
}

void PKBFacade::insertRead(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertEntity<int>(db->getReadManager(), stmt);
    }
}

std::vector<int> PKBFacade::getAllReadStmtNum() {
    return DatabaseUtils::getAllEntities<int>(db->getReadManager());
}

bool PKBFacade::containsRead(int stmtNum) {
    return DatabaseUtils::containsEntity<int>(db->getReadManager(), stmtNum);
}

bool PKBFacade::hasRead() {
    return DatabaseUtils::hasEntity<int>(db->getReadManager());
}

// Print
void PKBFacade::insertPrint(int stmtNum) {
    DatabaseUtils::insertEntity<int>(db->getPrintManager(), stmtNum);
}

void PKBFacade::insertPrint(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertEntity<int>(db->getPrintManager(), stmt);
    }
}

std::vector<int> PKBFacade::getAllPrintStmtNum() {
    return DatabaseUtils::getAllEntities<int>(db->getPrintManager());
}

bool PKBFacade::containsPrint(int stmtNum) {
    return DatabaseUtils::containsEntity<int>(db->getPrintManager(), stmtNum);
}

bool PKBFacade::hasPrint() {
    return DatabaseUtils::hasEntity<int>(db->getPrintManager());
}

// Call
void PKBFacade::insertCall(int stmtNum) {
    DatabaseUtils::insertEntity<int>(db->getCallManager(), stmtNum);
}

void PKBFacade::insertCall(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertEntity<int>(db->getCallManager(), stmt);
    }
}

std::vector<int> PKBFacade::getAllCallStmtNum() {
    return DatabaseUtils::getAllEntities<int>(db->getCallManager());
}

bool PKBFacade::containsCall(int stmtNum) {
    return DatabaseUtils::containsEntity<int>(db->getCallManager(), stmtNum);
}

bool PKBFacade::hasCall() {
    return DatabaseUtils::hasEntity<int>(db->getCallManager());
}

// While
void PKBFacade::insertWhile(int stmtNum) {
    DatabaseUtils::insertEntity<int>(db->getWhileManager(), stmtNum);
}

void PKBFacade::insertWhile(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertEntity<int>(db->getWhileManager(), stmt);
    }
}

std::vector<int> PKBFacade::getAllWhileStmtNum() {
    return DatabaseUtils::getAllEntities<int>(db->getWhileManager());
}

bool PKBFacade::containsWhile(int stmtNum) {
    return DatabaseUtils::containsEntity<int>(db->getWhileManager(), stmtNum);
}

bool PKBFacade::hasWhile() {
    return DatabaseUtils::hasEntity<int>(db->getWhileManager());
}

// If
void PKBFacade::insertIf(int stmtNum) {
    DatabaseUtils::insertEntity<int>(db->getIfManager(), stmtNum);
}

void PKBFacade::insertIf(const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertEntity<int>(db->getIfManager(), stmt);
    }
}

std::vector<int> PKBFacade::getAllIfStmtNum() {
    return DatabaseUtils::getAllEntities<int>(db->getIfManager());
}

bool PKBFacade::containsIf(int stmtNum) {
    return DatabaseUtils::containsEntity<int>(db->getIfManager(), stmtNum);
}

bool PKBFacade::hasIf() {
    return DatabaseUtils::hasEntity<int>(db->getIfManager());
}

// Follows
void PKBFacade::insertFollows(int followeeStmtNum, int followerStmtNum) {
    DatabaseUtils::insertRelationship<int, int>(db->getFollowsManager(), followeeStmtNum, followerStmtNum);
}

void PKBFacade::insertFollows(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) {
    for (const auto &follower : followerStmtSet) {
        DatabaseUtils::insertRelationship<int, int>(db->getFollowsManager(), followeeStmtNum, follower);
    }
}

bool PKBFacade::containsFollows(int followeeStmtNum) {
    return DatabaseUtils::containsKey<int, int>(db->getFollowsManager(), followeeStmtNum);
}


bool PKBFacade::containsFollowing(int followerStmtNum) {
    return DatabaseUtils::containsValue<int, int>(db->getFollowsManager(), followerStmtNum);
}

bool PKBFacade::containsFollowsRelationship(int followeeStmtNum, int followerStmtNum) {
    return DatabaseUtils::containsRelationship<int, int>(db->getFollowsManager(), followeeStmtNum, followerStmtNum);
}

bool PKBFacade::hasFollowsRelationship() {
    return DatabaseUtils::hasRelationship<int, int>(db->getFollowsManager());
}

std::vector<int> PKBFacade::getFollows(int followeeStmtNum) {
    return DatabaseUtils::getRelationship<int, int>(db->getFollowsManager(), followeeStmtNum);
}

std::vector<int> PKBFacade::getFollowing(int followerStmtNum) {
    return DatabaseUtils::getReverseRelationship<int, int>(db->getFollowsManager(), followerStmtNum);
}

std::vector<int> PKBFacade::getFolloweeStmts() {
    return DatabaseUtils::getKeys<int, int>(db->getFollowsManager());
}

std::vector<int> PKBFacade::getFollowerStmts() {
    return DatabaseUtils::getValues<int, int>(db->getFollowsManager());
}

// FollowsT
void PKBFacade::insertFollowsT(int followeeStmtNum, int followerStmtNum) {
    DatabaseUtils::insertRelationship<int, int>(db->getFollowsTManager(), followeeStmtNum, followerStmtNum);
}

void PKBFacade::insertFollowsT(int followeeStmtNum, const std::unordered_set<int>& followerStmtSet) {
    for (const auto &follower : followerStmtSet) {
        DatabaseUtils::insertRelationship<int, int>(db->getFollowsTManager(), followeeStmtNum, follower);
    }
}

bool PKBFacade::containsFollowsT(int followeeStmtNum) {
    return DatabaseUtils::containsKey<int, int>(db->getFollowsTManager(), followeeStmtNum);
}

bool PKBFacade::containsFollowingT(int followerStmtNum) {
    return DatabaseUtils::containsValue<int, int>(db->getFollowsTManager(), followerStmtNum);
}

bool PKBFacade::containsFollowsTRelationship(int followeeStmtNum, int followerStmtNum) {
    return DatabaseUtils::containsRelationship<int, int>(db->getFollowsTManager(), followeeStmtNum, followerStmtNum);
}

bool PKBFacade::hasFollowsTRelationship() {
    return DatabaseUtils::hasRelationship<int, int>(db->getFollowsTManager());
}

std::vector<int> PKBFacade::getFollowsT(int followeeStmtNum) {
    return DatabaseUtils::getRelationship<int, int>(db->getFollowsTManager(), followeeStmtNum);
}

std::vector<int> PKBFacade::getFollowingT(int followerStmtNum) {
    return DatabaseUtils::getReverseRelationship<int, int>(db->getFollowsTManager(), followerStmtNum);
}

std::vector<int> PKBFacade::getFolloweeTStmts() {
    return DatabaseUtils::getKeys<int, int>(db->getFollowsTManager());
}

std::vector<int> PKBFacade::getFollowerTStmts() {
    return DatabaseUtils::getValues<int, int>(db->getFollowsTManager());
}

// Parent
void PKBFacade::insertParent(int parentStmtNum, int childStmtNum) {
    DatabaseUtils::insertRelationship<int, int>(db->getParentManager(), parentStmtNum, childStmtNum);
}

void PKBFacade::insertParent(int parentStmtNum, const std::unordered_set<int>& childStmtSet) {
    for (const auto &child : childStmtSet) {
        DatabaseUtils::insertRelationship<int, int>(db->getParentManager(), parentStmtNum, child);
    }
}

bool PKBFacade::containsParent(int parentStmtNum) {
    return DatabaseUtils::containsKey<int, int>(db->getParentManager(), parentStmtNum);
}

bool PKBFacade::containsChild(int childStmtNum) {
    return DatabaseUtils::containsValue<int, int>(db->getParentManager(), childStmtNum);
}

bool PKBFacade::containsParentRelationship(int parentStmtNum, int childStmtNum) {
    return DatabaseUtils::containsRelationship<int, int>(db->getParentManager(), parentStmtNum, childStmtNum);
}

bool PKBFacade::hasParentRelationship() {
    return DatabaseUtils::hasRelationship<int, int>(db->getParentManager());
}

// Parent: set(child) : data
std::vector<int> PKBFacade::getChild(int parentStmtNum) {
    return DatabaseUtils::getRelationship<int, int>(db->getParentManager(), parentStmtNum);
}

// Child: set(parent) : reverseData
std::vector<int> PKBFacade::getParent(int childStmtNum) {
    return DatabaseUtils::getReverseRelationship<int, int>(db->getParentManager(), childStmtNum);
}

std::vector<int> PKBFacade::getParentStmts() {
    return DatabaseUtils::getKeys<int, int>(db->getParentManager());
}

std::vector<int> PKBFacade::getChildStmts() {
    return DatabaseUtils::getValues<int, int>(db->getParentManager());
}

// ParentT
void PKBFacade::insertParentT(int parentStmtNum, int childStmtNum) {
    DatabaseUtils::insertRelationship<int, int>(db->getParentTManager(), parentStmtNum, childStmtNum);
}

void PKBFacade::insertParentT(int parentStmtNum, const std::unordered_set<int>& childStmtSet) {
    for (const auto &child : childStmtSet) {
        DatabaseUtils::insertRelationship<int, int>(db->getParentTManager(), parentStmtNum, child);
    }
}

bool PKBFacade::containsParentT(int parentStmtNum) {
    return DatabaseUtils::containsKey<int, int>(db->getParentTManager(), parentStmtNum);
}

bool PKBFacade::containsChildT(int childStmtNum) {
    return DatabaseUtils::containsValue<int, int>(db->getParentTManager(), childStmtNum);
}

bool PKBFacade::containsParentTRelationship(int parentStmtNum, int childStmtNum) {
    return DatabaseUtils::containsRelationship<int, int>(db->getParentTManager(), parentStmtNum, childStmtNum);
}

bool PKBFacade::hasParentTRelationship() {
    return DatabaseUtils::hasRelationship<int, int>(db->getParentTManager());
}

// Parent: set(child) : data
std::vector<int> PKBFacade::getChildT(int parentStmtNum) {
    return DatabaseUtils::getRelationship<int, int>(db->getParentTManager(), parentStmtNum);
}

// Child: set(parent) : reverseData
std::vector<int> PKBFacade::getParentT(int childStmtNum) {
    return DatabaseUtils::getReverseRelationship<int, int>(db->getParentTManager(), childStmtNum);
}

std::vector<int> PKBFacade::getParentTStmts() {
    return DatabaseUtils::getKeys<int, int>(db->getParentTManager());
}

std::vector<int> PKBFacade::getChildTStmts() {
    return DatabaseUtils::getValues<int, int>(db->getParentTManager());
}


// Uses
void PKBFacade::insertUsesStmt(int stmtNum, std::string const & variable) {
    DatabaseUtils::insertRelationship<int, std::string>(db->getUsesStmtManager(), stmtNum, variable);
}

void PKBFacade::insertUsesStmt(int stmtNum, const std::unordered_set<std::string>& variableSet) {
    for (const auto &variable: variableSet) {
        DatabaseUtils::insertRelationship<int, std::string>(db->getUsesStmtManager(), stmtNum, variable);
    }
}

std::vector<std::string> PKBFacade::getUsesVariable(int stmtNum) {
    return DatabaseUtils::getRelationship<int, std::string>(db->getUsesStmtManager(), stmtNum);
}

std::vector<int> PKBFacade::getUsesStmt(std::string const & variable) {
    return DatabaseUtils::getReverseRelationship<int, std::string>(db->getUsesStmtManager(), variable);
}

std::vector<int> PKBFacade::getUsesStmt() {
    return DatabaseUtils::getKeys<int, std::string>(db->getUsesStmtManager());
}

std::vector<std::string> PKBFacade::getUsesVariable() {
    return DatabaseUtils::getValues<int, std::string>(db->getUsesStmtManager());
}

bool PKBFacade::containsUsesStmt(int stmtNum) {
    return DatabaseUtils::containsKey<int, std::string>(db->getUsesStmtManager(), stmtNum);
}

bool PKBFacade::containsUsesVariable(std::string const & variable) {
    return DatabaseUtils::containsValue<int, std::string>(db->getUsesStmtManager(), variable);
}

bool PKBFacade::containsUsesRelationship(int stmtNum, const std::string &variable) {
    return DatabaseUtils::containsRelationship<int, std::string>(db->getUsesStmtManager(), stmtNum, variable);
}

bool PKBFacade::hasUsesRelationship() {
    return DatabaseUtils::hasRelationship<int, std::string>(db->getUsesStmtManager());
}

// Modifies
void PKBFacade::insertModifiesStmt(int stmtNum, const std::string &variable) {
    DatabaseUtils::insertRelationship<int, std::string>(db->getModifiesStmtManager(), stmtNum, variable);
}

void PKBFacade::insertModifiesStmt(int stmtNum, const std::unordered_set<std::string>& variableSet) {
    for (const auto &variable: variableSet) {
        DatabaseUtils::insertRelationship<int, std::string>(db->getModifiesStmtManager(), stmtNum, variable);
    }
}

std::vector<std::string> PKBFacade::getModifiesVariable(int stmtNum) {
    return DatabaseUtils::getRelationship<int, std::string>(db->getModifiesStmtManager(), stmtNum);
}

std::vector<int> PKBFacade::getModifiesStmt(std::string const & variable) {
    return DatabaseUtils::getReverseRelationship<int, std::string>(db->getModifiesStmtManager(), variable);
}

std::vector<int> PKBFacade::getModifiesStmt() {
    return DatabaseUtils::getKeys<int, std::string>(db->getModifiesStmtManager());
}

std::vector<std::string> PKBFacade::getModifiesVariable() {
    return DatabaseUtils::getValues<int, std::string>(db->getModifiesStmtManager());
}

bool PKBFacade::containsModifiesStmt(int stmtNum) {
    return DatabaseUtils::containsKey<int, std::string>(db->getModifiesStmtManager(), stmtNum);
}

bool PKBFacade::containsModifiesVariable(std::string const & variable) {
    return DatabaseUtils::containsValue<int, std::string>(db->getModifiesStmtManager(), variable);
}

bool PKBFacade::containsModifiesRelationship(int stmtNum, std::string const & variable) {
    return DatabaseUtils::containsRelationship<int, std::string>(db->getModifiesStmtManager(), stmtNum, variable);
}

bool PKBFacade::hasModifiesRelationship() {
    return DatabaseUtils::hasRelationship<int, std::string>(db->getModifiesStmtManager());
}

// Procedure
// Uses Proc
void PKBFacade::insertUsesProc(std::string const & procedureName, std::string const & variable) {
    DatabaseUtils::insertRelationship<std::string, std::string>(db->getUsesProcManager(), procedureName, variable);
}

void PKBFacade::insertUsesProc(std::string const & procedureName, const std::unordered_set<std::string>& variableSet) {
    for (const auto &variable: variableSet) {
        DatabaseUtils::insertRelationship<std::string, std::string>(db->getUsesProcManager(), procedureName, variable);
    }
}

std::vector<std::string> PKBFacade::getUsesProcVariable(std::string const & procedureName) {
    return DatabaseUtils::getRelationship<std::string, std::string>(db->getUsesProcManager(), procedureName);
}

std::vector<std::string> PKBFacade::getUsesProcName(std::string const & variable) {
    return DatabaseUtils::getReverseRelationship<std::string, std::string>(db->getUsesProcManager(), variable);
}

std::vector<std::string> PKBFacade::getUsesProcVariable() {
    return DatabaseUtils::getValues<std::string, std::string>(db->getUsesProcManager());
}

std::vector<std::string> PKBFacade::getUsesProcName() {
    return DatabaseUtils::getKeys<std::string, std::string>(db->getUsesProcManager());
}

bool PKBFacade::containsUsesProc(std::string const & procedureName) {
    return DatabaseUtils::containsKey<std::string, std::string>(db->getUsesProcManager(), procedureName);
}

bool PKBFacade::containsUsesProcVariable(std::string const & variable) {
    return DatabaseUtils::containsValue<std::string, std::string>(db->getUsesProcManager(), variable);
}

bool PKBFacade::containsUsesProcRelationship(std::string const & procedureName, std::string const & variable) {
    return DatabaseUtils::containsRelationship<std::string, std::string>(db->getUsesProcManager(), procedureName, variable);
}

bool PKBFacade::hasUsesProcRelationship() {
    return DatabaseUtils::hasRelationship<std::string, std::string>(db->getUsesProcManager());
}

// Modifies Proc
void PKBFacade::insertModifiesProc(std::string const & procedureName, std::string const & variable) {
    DatabaseUtils::insertRelationship<std::string, std::string>(db->getModifiesProcManager(), procedureName, variable);
}

void PKBFacade::insertModifiesProc(std::string const & procedureName, const std::unordered_set<std::string>& variableSet) {
    for (const auto &variable: variableSet) {
        DatabaseUtils::insertRelationship<std::string, std::string>(db->getModifiesProcManager(), procedureName, variable);
    }
}

std::vector<std::string> PKBFacade::getModifiesProcVariable(std::string const & procedureName) {
    return DatabaseUtils::getRelationship<std::string, std::string>(db->getModifiesProcManager(), procedureName);
}

std::vector<std::string> PKBFacade::getModifiesProcName(std::string const & variable) {
    return DatabaseUtils::getReverseRelationship<std::string, std::string>(db->getModifiesProcManager(), variable);
}

std::vector<std::string> PKBFacade::getModifiesProcVariable() {
    return DatabaseUtils::getValues<std::string, std::string>(db->getModifiesProcManager());
}

std::vector<std::string> PKBFacade::getModifiesProcName() {
    return DatabaseUtils::getKeys<std::string, std::string>(db->getModifiesProcManager());
}

bool PKBFacade::containsModifiesProc(std::string const & procedureName) {
    return DatabaseUtils::containsKey<std::string, std::string>(db->getModifiesProcManager(), procedureName);
}

bool PKBFacade::containsModifiesProcVariable(std::string const & variable) {
    return DatabaseUtils::containsValue<std::string, std::string>(db->getModifiesProcManager(), variable);
}

bool PKBFacade::containsModifiesProcRelationship(std::string const & procedureName, std::string const & variable) {
    return DatabaseUtils::containsRelationship<std::string, std::string>(db->getModifiesProcManager(),
                                                                         procedureName, variable);
}

bool PKBFacade::hasModifiesProcRelationship() {
    return DatabaseUtils::hasRelationship<std::string, std::string>(db->getModifiesProcManager());
}

// Calls Proc
void PKBFacade::insertCallsProc(std::string const & caller, std::string const & callee) {
    DatabaseUtils::insertRelationship<std::string, std::string>(db->getCallsProcManager(), caller, callee);
}

void PKBFacade::insertCallsProc(std::string const & caller, const std::unordered_set<std::string>& calleeSet) {
    for (const auto &callee: calleeSet) {
        DatabaseUtils::insertRelationship<std::string, std::string>(db->getCallsProcManager(), caller, callee);
    }
}

std::vector<std::string> PKBFacade::getCallsProcCallee(std::string const & caller) {
    return DatabaseUtils::getRelationship<std::string, std::string>(db->getCallsProcManager(), caller);
}

std::vector<std::string> PKBFacade::getCallsProcCaller(std::string const & callee) {
    return DatabaseUtils::getReverseRelationship<std::string, std::string>(db->getCallsProcManager(), callee);
}

std::vector<std::string> PKBFacade::getCallsProcCaller() {
    return DatabaseUtils::getKeys<std::string, std::string>(db->getCallsProcManager());
}

std::vector<std::string> PKBFacade::getCallsProcCallee() {
    return DatabaseUtils::getValues<std::string, std::string>(db->getCallsProcManager());
}

bool PKBFacade::containsCallsProcCaller(std::string const & caller) {
    return DatabaseUtils::containsKey<std::string, std::string>(db->getCallsProcManager(), caller);
}

bool PKBFacade::containsCallsProcCallee(std::string const & callee) {
    return DatabaseUtils::containsValue<std::string, std::string>(db->getCallsProcManager(), callee);
}

bool PKBFacade::containsCallsProcRelationship(std::string const & caller, std::string const & callee) {
    return DatabaseUtils::containsRelationship<std::string, std::string>(db->getCallsProcManager(), caller, callee);
}

bool PKBFacade::hasCallsProcRelationship() {
    return DatabaseUtils::hasRelationship<std::string, std::string>(db->getCallsProcManager());
}


// CallsT Proc
void PKBFacade::insertCallsTProc(std::string const & caller, std::string const & callee) {
    DatabaseUtils::insertRelationship<std::string, std::string>(db->getCallsTProcManager(), caller, callee);
}

void PKBFacade::insertCallsTProc(std::string const & caller, const std::unordered_set<std::string>& calleeSet) {
    for (const auto &callee: calleeSet) {
        DatabaseUtils::insertRelationship<std::string, std::string>(db->getCallsTProcManager(), caller, callee);
    }
}

std::vector<std::string> PKBFacade::getCallsTProcCallee(std::string const & caller) {
    return DatabaseUtils::getRelationship<std::string, std::string>(db->getCallsTProcManager(), caller);
}

std::vector<std::string> PKBFacade::getCallsTProcCaller(std::string const & callee) {
    return DatabaseUtils::getReverseRelationship<std::string, std::string>(db->getCallsTProcManager(), callee);
}

std::vector<std::string> PKBFacade::getCallsTProcCaller() {
    return DatabaseUtils::getKeys<std::string, std::string>(db->getCallsTProcManager());
}

std::vector<std::string> PKBFacade::getCallsTProcCallee() {
    return DatabaseUtils::getValues<std::string, std::string>(db->getCallsTProcManager());
}

bool PKBFacade::containsCallsTProcCaller(std::string const & caller) {
    return DatabaseUtils::containsKey<std::string, std::string>(db->getCallsTProcManager(), caller);
}

bool PKBFacade::containsCallsTProcCallee(std::string const & callee) {
    return DatabaseUtils::containsValue<std::string, std::string>(db->getCallsTProcManager(), callee);
}

bool PKBFacade::containsCallsTProcRelationship(std::string const & caller, std::string const & callee) {
    return DatabaseUtils::containsRelationship<std::string, std::string>(db->getCallsTProcManager(), caller, callee);
}

bool PKBFacade::hasCallsTProcRelationship() {
    return DatabaseUtils::hasRelationship<std::string, std::string>(db->getCallsTProcManager());
}

// Assign Pattern
void PKBFacade::insertAssignPattern(std::string const & lhsVar, const size_t& rhsExprNodeHash, const int& stmtNum,
                                    const std::shared_ptr<ExprNode>& nodePtr) {
    DatabaseUtils::insertPattern<std::string, size_t, std::shared_ptr<ExprNode>>(db->getAssignPatternManager(),
            lhsVar, rhsExprNodeHash, stmtNum, nodePtr);
}

std::vector<std::string> PKBFacade::getAssignPatternLHS(size_t const & rhsExprNodeHash) {
    return DatabaseUtils::getPatternLHS<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), rhsExprNodeHash);
};

std::vector<size_t> PKBFacade::getAssignPatternRHS(std::string const & variable) {
    return DatabaseUtils::getPatternRHS<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), variable);
};

std::vector<std::string> PKBFacade::getAssignPatternLHS() {
    return DatabaseUtils::getAllPatternLHS<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager());
};

std::vector<size_t> PKBFacade::getAssignPatternRHS() {
    return DatabaseUtils::getAllPatternRHS<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager());
};

std::vector<int> PKBFacade::getAssignPatternStmtNum(std::string const & variable, size_t const & rhsExprNodeHash) {
    return DatabaseUtils::getPatternStmtNum<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), variable, rhsExprNodeHash);
};

std::vector<int> PKBFacade::getAssignPatternLHSStmtNum(std::string const & variable) {
    return DatabaseUtils::getPatternLHSStmtNum<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), variable);
};

std::vector<int> PKBFacade::getAssignPatternRHSStmtNum(size_t const & rhsExprNodeHash) {
    return DatabaseUtils::getPatternRHSStmtNum<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), rhsExprNodeHash);
};

std::vector<std::shared_ptr<ExprNode>> PKBFacade::getAssignPatternRHSExprNodePtr(size_t const & rhsExprNodeHash) {
    return DatabaseUtils::getPatternRHSNodePtr<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), rhsExprNodeHash);
};

std::vector<std::shared_ptr<ExprNode>> PKBFacade::getAssignPatternRHSExprNodePtr() {
    return DatabaseUtils::getPatternRHSNodePtr<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager());
};

bool PKBFacade::containsAssignPattern(std::string const & variable, size_t const & rhsExprNodeHash) {
    return DatabaseUtils::containsPattern<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), variable, rhsExprNodeHash);
};

bool PKBFacade::containsAssignPatternLHS(std::string const & variable) {
    return DatabaseUtils::containsPatternLHS<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), variable);
};

bool PKBFacade::containsAssignPatternRHS(size_t const & rhsExprNodeHash) {
    return DatabaseUtils::containsPatternRHS<std::string, size_t, std::shared_ptr<ExprNode>>(
            db->getAssignPatternManager(), rhsExprNodeHash);
};

bool PKBFacade::hasAssignPattern() {
    return DatabaseUtils::hasPattern<std::string, size_t, std::shared_ptr<ExprNode>>(db->getAssignPatternManager());
};

// If Pattern
void PKBFacade::insertIfPattern(std::string const & variable, const int& stmtNum) {
    DatabaseUtils::insertConditionalPattern<std::string, int>(db->getIfPatternManager(), variable, stmtNum);
}

void PKBFacade::insertIfPattern(std::string const & variable, const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertConditionalPattern<std::string, int>(db->getIfPatternManager(), variable, stmt);
    }
}

std::vector<int> PKBFacade::getIfPatternStmtNum(std::string const & variable) {
    return DatabaseUtils::getConditionalPatternValues<std::string, int>(
            db->getIfPatternManager(), variable);
};

std::vector<int> PKBFacade::getIfPatternStmtNum() {
    return DatabaseUtils::getConditionalPatternValues<std::string, int>(
            db->getIfPatternManager());
};

std::vector<std::string> PKBFacade::getIfPatternVariable(int stmtNum) {
    return DatabaseUtils::getConditionalPatternKeys<std::string, int>(
            db->getIfPatternManager(), stmtNum);
};

std::vector<std::string> PKBFacade::getIfPatternVariable() {
    return DatabaseUtils::getConditionalPatternKeys<std::string, int>(db->getIfPatternManager());
};


bool PKBFacade::containsIfPattern(std::string const & variable, int stmtNum) {
    return DatabaseUtils::containsConditionalPattern<std::string, int>(
            db->getIfPatternManager(), variable, stmtNum);
};

bool PKBFacade::containsIfPatternStmt(int stmtNum) {
    return DatabaseUtils::containsConditionalPatternValue<std::string, int>(
            db->getIfPatternManager(), stmtNum);
};

bool PKBFacade::containsIfPatternVariable(std::string const & variable) {
    return DatabaseUtils::containsConditionalPatternKey<std::string, int>(
            db->getIfPatternManager(), variable);
};

bool PKBFacade::hasIfPattern() {
    return DatabaseUtils::hasConditionalPattern<std::string, int>(
            db->getIfPatternManager());
};

// While Pattern
void PKBFacade::insertWhilePattern(std::string const & variable, const int& stmtNum) {
    DatabaseUtils::insertConditionalPattern<std::string, int>(db->getWhilePatternManager(), variable, stmtNum);
}

void PKBFacade::insertWhilePattern(std::string const & variable, const std::unordered_set<int>& stmtSet) {
    for (const auto &stmt : stmtSet) {
        DatabaseUtils::insertConditionalPattern<std::string, int>(db->getWhilePatternManager(), variable, stmt);
    }
}

std::vector<std::string> PKBFacade::getWhilePatternVariable(int stmtNum) {
    return DatabaseUtils::getConditionalPatternKeys<std::string, int>(
            db->getWhilePatternManager(), stmtNum);
};

std::vector<int> PKBFacade::getWhilePatternStmtNum(std::string const & variable) {
    return DatabaseUtils::getConditionalPatternValues<std::string, int>(
            db->getWhilePatternManager(), variable);
};

std::vector<std::string> PKBFacade::getWhilePatternVariable() {
    return DatabaseUtils::getConditionalPatternKeys<std::string, int>(db->getWhilePatternManager());
};

std::vector<int> PKBFacade::getWhilePatternStmtNum() {
    return DatabaseUtils::getConditionalPatternValues<std::string, int>(
            db->getWhilePatternManager());
};

bool PKBFacade::containsWhilePattern(std::string const & variable, int stmtNum) {
    return DatabaseUtils::containsConditionalPattern<std::string, int>(
            db->getWhilePatternManager(), variable, stmtNum);
};

bool PKBFacade::containsWhilePatternStmt(int stmtNum) {
    return DatabaseUtils::containsConditionalPatternValue<std::string, int>(
            db->getWhilePatternManager(), stmtNum);
};

bool PKBFacade::containsWhilePatternVariable(std::string const & variable) {
    return DatabaseUtils::containsConditionalPatternKey<std::string, int>(
            db->getWhilePatternManager(), variable);
};

bool PKBFacade::hasWhilePattern() {
    return DatabaseUtils::hasConditionalPattern<std::string, int>(
            db->getWhilePatternManager());
};

