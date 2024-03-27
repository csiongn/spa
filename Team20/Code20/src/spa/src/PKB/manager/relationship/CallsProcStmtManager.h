#pragma once

#include <string>

#include "RelationshipManager.h"

// To support attributes
// callee proc , stmt_no
class CallsProcStmtManager : public RelationshipManager<std::string, int> {
};