#pragma once

#include "RelationshipManager.h"

// stmtNo, string? -> variables
// reverse will be string, stmtNo, which will be which stmt modifies the variable
class ModifiesStmtManager : public RelationshipManager<int, std::string> {
};
