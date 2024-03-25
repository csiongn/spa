#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "PKB/facade/IPKBWriter.h"

class CFGNode; // Forward declaration as there is circular dependency between CFGNode <-> ICFGVisitor

#include "SP/control_flow_graph/CFG.h"

class ICFGVisitor {
 public:
    virtual ~ICFGVisitor() = default;

    virtual void visit(const std::shared_ptr<CFGNode>& node) = 0;
    virtual void pushToPKB(const std::shared_ptr<IPKBWriter>& pkbWriter) = 0;
};
