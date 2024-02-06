#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "manager/relationship/FollowsManager.h"
// Database class contains all our tables/repositories
class Database {

// Database tables
private:
    std::shared_ptr<FollowsManager> followsManager = std::make_shared<FollowsManager>();;

public:
    // Get the follows table
    std::shared_ptr<FollowsManager> getFollowsManager();

};
