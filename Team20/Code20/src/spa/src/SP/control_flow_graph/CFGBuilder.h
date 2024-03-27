#pragma once

#include <memory>
#include <vector>

#include "SP/AST.h"
#include "CFG.h"

class CFGBuilder {
    static std::shared_ptr<CFG> buildCFGProc(const ProcedureNode& procedureNode);
    static std::vector<std::shared_ptr<CFGNode>> processBlockNode(const std::shared_ptr<BlockNode>& block, const std::vector<std::shared_ptr<CFGNode>>& prevNodes,
        const std::shared_ptr<CFG>& cfg);
    static std::vector<std::shared_ptr<CFGNode>> processIfNode(const std::shared_ptr<IfNode>& node, const std::vector<std::shared_ptr<CFGNode>> prevNodes, const std::shared_ptr<CFG>& cfg);
    static std::vector<std::shared_ptr<CFGNode>> processWhileNode(const std::shared_ptr<WhileNode>& node, const std::vector<std::shared_ptr<CFGNode>> prevNodes, const std::shared_ptr<CFG>& cfg);
    static void connectNodes(const std::vector<std::shared_ptr<CFGNode>>& fromNodes, const std::vector<std::shared_ptr<CFGNode>>& toNodes);

 public:
    static std::shared_ptr<CFGManager> buildCFG(const ProgramNode& programNode);

};