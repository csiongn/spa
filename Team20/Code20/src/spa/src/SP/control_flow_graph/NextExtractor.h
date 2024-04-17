#pragma once

#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "ICFGVisitor.h"
#include "SP/CallGraph.h"

class NextExtractor : public ICFGVisitor {
    std::unordered_map<int, std::vector<int>> nextRelationships;
    std::unordered_map<int, std::vector<int>> revNextRelationships;
    std::vector<std::bitset<MAX_NODES>> adjMatrix;
    std::unordered_set<int> visited;
    bool finalized = false;

 public:
    void visit(const std::shared_ptr<CFGNode>& node);
    void visit(CFG& cfg) override;
    void pushToPKB(const std::shared_ptr<IPKBWriter> &pkbWriter);
    const std::unordered_map<int, std::vector<int>>& getNextRelationships() const;
    void finalize();

    // Run-time methods
    bool hasNextT() const;
    bool containsNextTFrom(int stmtNo) const;
    bool containsNextTTo(int stmtNo) const;
    bool containsNextT(int stmtNoFrom, int stmtNoTo) const;
    std::vector<int> getStatementsNextTFrom();
    std::vector<int> getStatementsNextTTo();
    std::vector<int> getStatementsNextTFrom(int stmtNum);
    std::vector<int> getStatementsNextTTo(int stmtNum);
    std::vector<std::pair<int, int> > getNextT();
};
