#pragma once

#include <string>

#include "ConditionalPatternManager.h"

// T need to be stmt_no and U is ExprNode's Hash size_t, V is ptr
// Stmt_No, RHS Node Hash Value, Ptr to Node
// Holds controls variables
class WhilePatternManager : public ConditionalPatternManager<std::string, int> {
};