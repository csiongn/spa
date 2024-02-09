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

// Database class contains all our tables/repositories
class Database {

// Database tables
private:
    // Entity(Non-Statement)
    std::shared_ptr<VariableManager> variableManager = std::make_shared<VariableManager>();

    // Entity(Statement)
    std::shared_ptr<AssignManager> assignManager = std::make_shared<AssignManager>();

    // Relationship
    std::shared_ptr<FollowsManager> followsManager = std::make_shared<FollowsManager>();

    std::shared_ptr<FollowsTManager> followsTManager = std::make_shared<FollowsTManager>();

    std::shared_ptr<ParentManager> parentManager = std::make_shared<ParentManager>();

    std::shared_ptr<ParentTManager> parentTManager = std::make_shared<ParentTManager>();


public:

    std::shared_ptr<VariableManager> getVariableManager();
    std::shared_ptr<AssignManager> getAssignManager();
    std::shared_ptr<FollowsManager> getFollowsManager();
    std::shared_ptr<FollowsTManager> getFollowsTManager();
    std::shared_ptr<ParentManager> getParentManager();
    std::shared_ptr<ParentTManager> getParentTManager();

};
