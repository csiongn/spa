#pragma once

#include <memory>
#include <string>

#include "PatternManager.h"

// Used for partial pattern matching, will contain possible subsets of expr/nodes from a full exprNode
// T, U need to be var : string and U is ExprNode's Hash size_t, V is ptr
// LHS Variable Name, RHS Node Hash Value, Ptr to Node
// Still store reference to ExprNode, given that BinaryNode and UnaryNode are subclasses of ExprNode
class AssignPartialPatternManager : public PatternManager<std::string, size_t, std::shared_ptr<ExprNode>> {
};
