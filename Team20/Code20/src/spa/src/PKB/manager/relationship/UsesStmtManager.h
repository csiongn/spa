#pragma once

#include "RelationshipManager.h"

// stmtNo, string? -> variables
class UsesStmtManager : public RelationshipManager<int, std::string> {
};

