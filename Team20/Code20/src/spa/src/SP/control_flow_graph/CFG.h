#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "SP/AST.h"

class ICFGVisitor;

#include "SP/control_flow_graph/ICFGVisitor.h"

class NumberMap {
    // Augmented tree for storing starting statement number for every procedure together with name
 private:
    std::map<int, std::string> start;
    // Uses std::map which is ordered using a red-black tree (see: https://en.cppreference.com/w/cpp/container/map)

 public:
    NumberMap() = default;

    void addStartNum(int stmtNum, const std::string& procName) {
        start[stmtNum] = procName;
    }

    std::string findProcName(int stmtNum) {
        auto it = start.lower_bound(stmtNum);

        if (it == start.begin()) {
            return ""; // No number smaller than the given number
        }

        --it;
        return it->second;
    }
};

class CFGNode : public std::enable_shared_from_this<CFGNode> {
 public:
    // Attributes
    int stmtNum;
    std::vector<std::shared_ptr<CFGNode>> next;
    // std::unordered_set<std::string> uses;
    // std::unordered_set<std::string> modifies;

    // Constructor
    explicit CFGNode(int stmtNum) : stmtNum(stmtNum) {
    }

    // Methods
    std::vector<std::shared_ptr<CFGNode>> getNext() { return next; }
    void addNext(const std::shared_ptr<CFGNode>& node) { next.emplace_back(node); }

    // Accept method for visitor
    void accept(ICFGVisitor& visitor);
};

class CFG {
 public:
    // Attributes
    std::string procName;
    int startNum;
    std::shared_ptr<CFGNode> root;
    std::unordered_map<int, std::shared_ptr<CFGNode>> stmtNumToNode;

    // Constructor
    CFG() = default;

    explicit CFG(std::string procName, int startNum, std::shared_ptr<CFGNode> root) : procName(std::move(procName)),
        startNum(startNum), root(std::move(root)) {
    }

    // Methods
    std::shared_ptr<CFGNode> findNode(const int stmtNum) { return stmtNumToNode[stmtNum]; }
    void addNode(const int stmtNum, std::shared_ptr<CFGNode> cfgNode) { stmtNumToNode[stmtNum] = std::move(cfgNode); }

    void accept(ICFGVisitor& visitor) const;
};

class CFGManager {
 public:
    // Attributes
    std::unordered_map<std::string, std::shared_ptr<CFG>> cfgs;
    std::unique_ptr<NumberMap> startStmtNumToCFG;

    // Constructor
    CFGManager() : startStmtNumToCFG(std::make_unique<NumberMap>()) {}

    // Methods
    void addCFG(const std::string& name, std::shared_ptr<CFG> cfg, int startStmtNum) {
        cfgs[name] = cfg;
        startStmtNumToCFG->addStartNum(startStmtNum, name);
    };

    std::shared_ptr<CFGNode> getCFGNode(const int stmtNum) {
        auto cfgName = startStmtNumToCFG->findProcName(stmtNum);
        return cfgs[cfgName]->findNode(stmtNum);
    }

    std::shared_ptr<CFG> getCFG(const int stmtNum) {
        auto cfgName = startStmtNumToCFG->findProcName(stmtNum);
        return cfgs[cfgName];
    }

    void accept(ICFGVisitor& visitor) const;

    std::unordered_map<std::string, std::shared_ptr<CFG>> getCFGs() { return cfgs; }
};
