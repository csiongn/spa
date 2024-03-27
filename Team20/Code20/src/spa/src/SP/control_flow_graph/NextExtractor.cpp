#include <unordered_map>
#include <vector>
#include <set>

#include "NextExtractor.h"
#include "ICFGVisitor.h"
#include "CFG.h"

void NextExtractor::visit(const std::shared_ptr<CFGNode>& node) {
    if (visited.insert(node->stmtNum).second) { // Insert returns a pair, .second is true if the element was inserted
        for (const auto& nextNode : node->getNext()) {
            nextRelationships[node->stmtNum].push_back(nextNode->stmtNum);
            nextNode->accept(*this); // Recursively visit next nodes
        }
    }
}

const std::unordered_map<int, std::vector<int>>& NextExtractor::getNextRelationships() const {
    return nextRelationships;
}

void NextExtractor::pushToPKB(const std::shared_ptr<IPKBWriter> &pkbWriter) {
    for (const auto pair : nextRelationships) {
        for (const auto nextStmt : pair.second) {
            pkbWriter->insertNext(pair.first, nextStmt);
        }
    }
}

