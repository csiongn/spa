#pragma once

#include "PatternManager.h"

// Used for Exact Pattern Matching
// T, U need to be var : string and U is ExprNode's Hash size_t, V is ptr
// LHS Variable Name, RHS Node Hash Value, Ptr to Node
class AssignPatternManager : public PatternManager<std::string, size_t, std::shared_ptr<ExprNode>> {
};
