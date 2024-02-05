#pragma once

// FollowsManager inherits Relationship Repository and is responsible for storing and managing the Follows relationship between statements.

#include "PKB/manager/relationship/RelationshipManager.h"

class FollowsManager : public RelationshipManager<int, int> {
};