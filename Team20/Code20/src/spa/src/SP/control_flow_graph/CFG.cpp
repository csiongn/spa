
#include "CFG.h"

void CFGNode::accept(ICFGVisitor& visitor)  {
    visitor.visit(shared_from_this());
}

void CFG::accept(ICFGVisitor &visitor) const {
    root->accept(visitor);
}

void CFGManager::accept(ICFGVisitor &visitor) const {
    for (auto it = cfgs.begin(); it != cfgs.end(); ++it) {
        // Iterate through all CFGS
        it->second->accept(visitor); // Make the visitor visit all CFGs
    }
}
