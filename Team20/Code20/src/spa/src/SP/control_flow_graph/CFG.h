#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Models/SimpleProgram.h"
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

        // If iterator is at the beginning and key doesn't match stmtNum,
        // stmtNum is before the first stored procedure
        if (it == start.begin() && it->first != stmtNum) {
            return "";
        } else if (it == start.begin()) { // If it matches the first procedure start
            return it->second;
        }

        // If iterator goes beyond the last element (no procedure starts after stmtNum),
        // stmtNum belongs to the last procedure.
        if (it == start.end()) {
            --it; // Move back to the last procedure's start
            return it->second;
        }

        // If stmtNum is greater than the found procedure's start,
        // but there exists another procedure starting after stmtNum,
        // then the stmtNum belongs to the procedure prior to it.
        if (it->first > stmtNum) {
            --it;
        }

        return it->second;
    }
};

class CFGNode : public std::enable_shared_from_this<CFGNode> {
 public:
    // Attributes
    int stmtNum;
    SimpleProgram::StatementType stmtType;
    std::vector<std::shared_ptr<CFGNode>> previous;
    std::vector<std::shared_ptr<CFGNode>> next;
    const std::unordered_set<std::string> uses;
    const std::unordered_set<std::string> modifies;

    // Constructor
    explicit CFGNode(int stmtNum, SimpleProgram::StatementType stmtType, const std::unordered_set<std::string>& uses, const std::unordered_set<std::string>& modifies) :
    stmtNum(stmtNum), stmtType(stmtType), uses(uses), modifies(modifies) {
    }

    // Methods
    std::vector<std::shared_ptr<CFGNode>> getNext() { return next; }
    std::vector<std::shared_ptr<CFGNode>> getPrevious() { return previous; }
    void addNext(const std::shared_ptr<CFGNode>& node) { next.emplace_back(node); }
    void addPrevious(const std::shared_ptr<CFGNode>& node) { previous.emplace_back(node); }

};

class CFG : public std::enable_shared_from_this<CFG> {
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
    std::shared_ptr<CFGNode> findNode(const int stmtNum) {
        if (!(stmtNumToNode.count(stmtNum))) {
            return nullptr;
        }
        return stmtNumToNode[stmtNum];
    }
    void addNode(const int stmtNum, std::shared_ptr<CFGNode> cfgNode) { stmtNumToNode[stmtNum] = std::move(cfgNode); }

    void accept(ICFGVisitor& visitor);
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

    std::string getCFGName(const int stmtNum) {
        return startStmtNumToCFG->findProcName(stmtNum);
    }

    void accept(ICFGVisitor& visitor) const;

    std::unordered_map<std::string, std::shared_ptr<CFG>> getCFGs() { return cfgs; }
};
