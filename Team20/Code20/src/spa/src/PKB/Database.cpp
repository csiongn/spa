#include "Database.h"

std::shared_ptr<AssignManager> Database::getAssignManager() {
    return assignManager;
}

std::shared_ptr<VariableManager> Database::getVariableManager() {
    return variableManager;
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