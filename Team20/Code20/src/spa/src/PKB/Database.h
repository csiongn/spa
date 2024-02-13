#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "PKB/manager/relationship/FollowsManager.h"
#include "PKB/manager/entity/VariableManager.h"
#include "PKB/manager/entity/AssignManager.h"
#include "PKB/manager/relationship/FollowsTManager.h"
#include "PKB/manager/relationship/ParentManager.h"
#include "PKB/manager/relationship/ParentTManager.h"
#include "PKB/manager/entity/IfManager.h"
#include "PKB/manager/entity/PrintManager.h"
#include "PKB/manager/entity/ReadManager.h"
#include "PKB/manager/entity/WhileManager.h"
#include "PKB/manager/entity/CallManager.h"
#include "PKB/manager/entity/ConstantManager.h"
#include "PKB/manager/entity/ProcedureManager.h"
#include "PKB/manager/entity/StatementManager.h"

// Database class contains all our tables/repositories
class Database {

// Database tables
private:
    // Entity(Non-Statement)
    std::shared_ptr<ProcedureManager> procedureManager = std::make_shared<ProcedureManager>();
    std::shared_ptr<VariableManager> variableManager = std::make_shared<VariableManager>();
    std::shared_ptr<ConstantManager> constantManager = std::make_shared<ConstantManager>();

    // Entity(Statement)
    std::shared_ptr<StatementManager> statementManager = std::make_shared<StatementManager>();
    std::shared_ptr<AssignManager> assignManager = std::make_shared<AssignManager>();
    std::shared_ptr<ReadManager> readManager = std::make_shared<ReadManager>();
    std::shared_ptr<PrintManager> printManager = std::make_shared<PrintManager>();
    std::shared_ptr<CallManager> callManager = std::make_shared<CallManager>();
    std::shared_ptr<WhileManager> whileManager = std::make_shared<WhileManager>();
    std::shared_ptr<IfManager> ifManager = std::make_shared<IfManager>();


    // Relationship
    std::shared_ptr<FollowsManager> followsManager = std::make_shared<FollowsManager>();

    std::shared_ptr<FollowsTManager> followsTManager = std::make_shared<FollowsTManager>();

    std::shared_ptr<ParentManager> parentManager = std::make_shared<ParentManager>();

    std::shared_ptr<ParentTManager> parentTManager = std::make_shared<ParentTManager>();


public:

    std::shared_ptr<ProcedureManager> getProcedureManager();
    std::shared_ptr<VariableManager> getVariableManager();
    std::shared_ptr<ConstantManager> getConstantManager();

    std::shared_ptr<StatementManager> getStatementManager();
    std::shared_ptr<AssignManager> getAssignManager();
    std::shared_ptr<ReadManager> getReadManager();
    std::shared_ptr<PrintManager> getPrintManager();
    std::shared_ptr<CallManager> getCallManager();
    std::shared_ptr<WhileManager> getWhileManager();
    std::shared_ptr<IfManager> getIfManager();

    std::shared_ptr<FollowsManager> getFollowsManager();
    std::shared_ptr<FollowsTManager> getFollowsTManager();
    std::shared_ptr<ParentManager> getParentManager();
    std::shared_ptr<ParentTManager> getParentTManager();

};
