#pragma once

#include "ConditionalPatternManager.h"

// T need to be stmt_no and U is ExprNode's Hash size_t, V is ptr
// Stmt_No, Variable String, Ptr to Node
// Holds controls variables
class IfPatternManager : public ConditionalPatternManager<std::string, int> {
};