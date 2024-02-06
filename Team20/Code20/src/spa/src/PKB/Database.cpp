#include "Database.h"

std::shared_ptr<FollowsManager> Database::getFollowsManager() {
    return followsManager;
}