#include <algorithm>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "CFGBuilder.h"
#include "CFG.h"


std::shared_ptr<CFGManager> CFGBuilder::buildCFG() {
    auto cfgManager = std::make_shared<CFGManager>();
    for (const auto &procedure: programNode.procedures) {
        auto cfg = buildCFGProc(*procedure);
        cfgManager->addCFG(cfg->procName, cfg, cfg->startNum);
    }
    return cfgManager;
}

std::shared_ptr<CFG> CFGBuilder::buildCFGProc(const ProcedureNode &procedureNode) {
    std::shared_ptr<BlockNode> body = procedureNode.body;
    std::vector<std::shared_ptr<StmtNode> > statements = body->statements;
    if (statements.empty()) {
        return nullptr;
    }
    const auto entryNode = statements.front();
    auto uses = usesMap[entryNode->stmtNumber];
    const auto entryCFGNode = std::make_shared<CFGNode>(entryNode->stmtNumber, entryNode->stmtType,
        usesMap[entryNode->stmtNumber], modifiesMap.at(entryNode->stmtNumber));
    auto cfg = std::make_shared<CFG>(procedureNode.name, entryNode->stmtNumber, entryCFGNode);
    cfg->addNode(entryNode->stmtNumber, entryCFGNode);
    processBlockNode(body, {cfg->root}, cfg);

    return cfg;
}

std::vector<std::shared_ptr<CFGNode> > CFGBuilder::processBlockNode(const std::shared_ptr<BlockNode> &block,
                                                                    const std::vector<std::shared_ptr<CFGNode> > &
                                                                    prevNodes,
                                                                    const std::shared_ptr<CFG> &cfg) {
    std::vector<std::shared_ptr<CFGNode> > lastNodes = prevNodes;
    for (const auto &statement: block->statements) {
        if (lastNodes.front()->stmtNum == statement->stmtNumber) {
            // Skip processing the first node as it has been handled in buildCFGProc
            continue;
        }
        if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(statement)) {
            lastNodes = processWhileNode(whileNode, lastNodes, cfg);
        } else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(statement)) {
            lastNodes = processIfNode(ifNode, lastNodes, cfg);
        } else {
            auto newNode = std::make_shared<CFGNode>(statement->stmtNumber, statement->stmtType,
                usesMap[statement->stmtNumber], modifiesMap[statement->stmtNumber]);
            cfg->addNode(statement->stmtNumber, newNode);
            connectNodes(lastNodes, {newNode});
            lastNodes = {newNode};
        }
    }

    return lastNodes;
}

std::vector<std::shared_ptr<CFGNode> > CFGBuilder::processIfNode(const std::shared_ptr<IfNode> &node,
                                                                 const std::vector<std::shared_ptr<CFGNode> > prevNodes,
                                                                 const std::shared_ptr<CFG> &cfg) {
    auto ifNode = std::make_shared<CFGNode>(node->stmtNumber, SimpleProgram::StatementType::IF, usesMap.at(node->stmtNumber), modifiesMap.at(node->stmtNumber));
    cfg->addNode(node->stmtNumber, ifNode);
    connectNodes(prevNodes, {ifNode});
    auto thenBranchNodes = processBlockNode(node->thenBranch, {ifNode}, cfg);
    auto elseBranchNodes = processBlockNode(node->elseBranch, {ifNode}, cfg);
    thenBranchNodes.insert(thenBranchNodes.end(), elseBranchNodes.begin(), elseBranchNodes.end()); // Set of last nodes combining both branches

    return thenBranchNodes;
}

std::vector<std::shared_ptr<CFGNode> > CFGBuilder::processWhileNode(const std::shared_ptr<WhileNode> &node,
                                                                    const std::vector<std::shared_ptr<CFGNode> >
                                                                    prevNodes, const std::shared_ptr<CFG> &cfg) {
    auto whileNode = std::make_shared<CFGNode>(node->stmtNumber, SimpleProgram::StatementType::WHILE, usesMap.at(node->stmtNumber), modifiesMap.at(node->stmtNumber));
    cfg->addNode(node->stmtNumber, whileNode);
    connectNodes(prevNodes, {whileNode});
    auto loopNodes = processBlockNode(node->body, {whileNode}, cfg);
    connectNodes({loopNodes.back()}, {whileNode});

    return {whileNode};
}

void CFGBuilder::connectNodes(const std::vector<std::shared_ptr<CFGNode> > &fromNodes,
                              const std::vector<std::shared_ptr<CFGNode> > &toNodes) {
    for (auto &fromNode: fromNodes) {
        for (auto &toNode: toNodes) {
            if (fromNode && toNode) {
                // Check for null pointers
                fromNode->addNext(toNode);
                toNode->addPrevious(fromNode);
            }
        }
    }
}
