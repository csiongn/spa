#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "PKB/facade/IPKBWriter.h"

class CFG; // Forward declaration as there is circular dependency between CFG <-> ICFGVisitor

#include "SP/control_flow_graph/CFG.h"

class ICFGVisitor {
 public:
    virtual ~ICFGVisitor() = default;

    virtual void visit(CFG& cfg) = 0;
};
