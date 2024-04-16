#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "SP/AST.h"
#include "CFG.h"

class CFGBuilder {

  // Attributes
  std::unordered_map<int, std::unordered_set<std::string>>& usesMap;
  std::unordered_map<int, std::unordered_set<std::string>>& modifiesMap;
  const ProgramNode& programNode;

  // Constructor
  std::shared_ptr<CFG> buildCFGProc(const ProcedureNode& procedureNode);
  std::vector<std::shared_ptr<CFGNode>> processBlockNode(const std::shared_ptr<BlockNode>& block, const std::vector<std::shared_ptr<CFGNode>>& prevNodes,
        const std::shared_ptr<CFG>& cfg);
  std::vector<std::shared_ptr<CFGNode>> processBlockNodeHelper(const std::shared_ptr<StmtNode>& statement,
                                                               const std::vector<std::shared_ptr<CFGNode>>& prevNodes,
                                                               const std::shared_ptr<CFG>& cfg);
  std::vector<std::shared_ptr<CFGNode>> processIfNode(const std::shared_ptr<IfNode>& node, const std::vector<std::shared_ptr<CFGNode>> prevNodes, const std::shared_ptr<CFG>& cfg);
  std::vector<std::shared_ptr<CFGNode>> processWhileNode(const std::shared_ptr<WhileNode>& node, const std::vector<std::shared_ptr<CFGNode>> prevNodes, const std::shared_ptr<CFG>& cfg);
  void connectNodes(const std::vector<std::shared_ptr<CFGNode>>& fromNodes, const std::vector<std::shared_ptr<CFGNode>>& toNodes);

 public:
  CFGBuilder(std::unordered_map<int, std::unordered_set<std::string>>& usesMap,
  std::unordered_map<int, std::unordered_set<std::string>>& modifiesMap,
    const ProgramNode& programNode) : usesMap(usesMap), modifiesMap(modifiesMap), programNode(programNode) {}
  std::shared_ptr<CFGManager> buildCFG();
};
