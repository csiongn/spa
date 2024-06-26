#include "Database.h"

std::shared_ptr<ProcedureManager> Database::getProcedureManager() {
  return procedureManager;
}

std::shared_ptr<VariableManager> Database::getVariableManager() {
  return variableManager;
}

std::shared_ptr<ConstantManager> Database::getConstantManager() {
  return constantManager;
}

std::shared_ptr<StatementManager> Database::getStatementManager() {
  return statementManager;
}

std::shared_ptr<AssignManager> Database::getAssignManager() {
  return assignManager;
}

std::shared_ptr<ReadManager> Database::getReadManager() {
  return readManager;
}

std::shared_ptr<PrintManager> Database::getPrintManager() {
  return printManager;
}

std::shared_ptr<CallManager> Database::getCallManager() {
  return callManager;
}

std::shared_ptr<WhileManager> Database::getWhileManager() {
  return whileManager;
}

std::shared_ptr<IfManager> Database::getIfManager() {
  return ifManager;
}

std::shared_ptr<FollowsManager> Database::getFollowsManager() {
  return followsManager;
}

std::shared_ptr<FollowsTManager> Database::getFollowsTManager() {
  return followsTManager;
}

std::shared_ptr<ParentManager> Database::getParentManager() {
  return parentManager;
}

std::shared_ptr<ParentTManager> Database::getParentTManager() {
  return parentTManager;
}

std::shared_ptr<UsesStmtManager> Database::getUsesStmtManager() {
  return usesStmtManager;
}

std::shared_ptr<ModifiesStmtManager> Database::getModifiesStmtManager() {
  return modifiesStmtManager;
}

std::shared_ptr<UsesProcManager> Database::getUsesProcManager() {
  return usesProcManager;
}

std::shared_ptr<ModifiesProcManager> Database::getModifiesProcManager() {
  return modifiesProcManager;
}

std::shared_ptr<CallsProcManager> Database::getCallsProcManager() {
  return callsProcManager;
}

std::shared_ptr<CallsTProcManager> Database::getCallsTProcManager() {
  return callsTProcManager;
}

std::shared_ptr<NextManager> Database::getNextManager() {
  return nextManager;
}

std::shared_ptr<PrintVarManager> Database::getPrintVarManager() {
  return printVarManager;
}

std::shared_ptr<ReadVarManager> Database::getReadVarManager() {
  return readVarManager;
}

std::shared_ptr<CallsProcStmtManager> Database::getCallsProcStmtManager() {
  return callsProcStmtManager;
}

std::shared_ptr<AssignPatternManager> Database::getAssignPatternManager() {
  return assignPatternManager;
}

std::shared_ptr<AssignPartialPatternManager> Database::getAssignPartialPatternManager() {
  return assignPartialPatternManager;
}

std::shared_ptr<IfPatternManager> Database::getIfPatternManager() {
  return ifPatternManager;
}

std::shared_ptr<WhilePatternManager> Database::getWhilePatternManager() {
  return whilePatternManager;
}

std::shared_ptr<RuntimeManager> Database::getRuntimeManager() {
  return runtimeManager;
}