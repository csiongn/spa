#include "DesignExtractor.h"

#include <iostream>

void DesignExtractor::extractDesign(const ProgramNode& astRoot) {
    visitProgramNode(astRoot);
    pushToPKB();
}

void DesignExtractor::pushToPKB() {
    for (const auto& pair: follows) {
        pkbFacade->insertFollows(pair.second, pair.first);
    }

    for (const auto& pair: parent) {
        pkbFacade->insertParent(pair.second, pair.first);
    }

    for (const auto& pair : followsT) { // Iterate through the map of all followsT relationships
        for (const auto& followeeStmtNum : pair.second) { // Iterate through pair.second – the set of statements that the key statement transitively follows
            pkbFacade->insertFollowsT(followeeStmtNum, pair.first);
        }
    }

    for (const auto& pair : parentT) { // Iterate through the map of all parentT relationships
        for (const auto& parentStmtNum : pair.second) { // Iterate through pair.second – the set of statements that is a transitive parent of the key statement
            pkbFacade->insertParentT(parentStmtNum, pair.first);
        }
    }
}


void DesignExtractor::visitProgramNode(const ProgramNode& node) {
    for (const auto& procNode : node.procedures) {
        visitProcedureNode(*procNode);
    }
}

void DesignExtractor::visitProcedureNode(const ProcedureNode& node) {
    std::vector<int> stmtList; // Tracks statement numbers within the current scope for 'Follows' relationship
    visitBlockNode(*node.body, 0, stmtList); // 0 is used as parentStmt for the top level
}

void DesignExtractor::visitBlockNode(const BlockNode& node, int parentStmt, std::vector<int>& stmtList) {
    for (const auto& stmt : node.statements) {
        visitStmtNode(*stmt, parentStmt, stmtList);
    }
}

void DesignExtractor::visitStmtNode(const StmtNode& node, int parentStmt, std::vector<int>& stmtList) {
    int stmtNumber = ++currentStmtNumber; // Assign and increment the global statement number
    updateParent(stmtNumber, parentStmt); // Update the 'Parent' relationship
    updateFollows(stmtNumber, stmtList); // Update the 'Follows' relationship
    stmtList.push_back(stmtNumber); // Add current statement to the list for potential 'Follows' relationships

    if (const auto* ifNode = dynamic_cast<const IfNode*>(&node)) {
        visitIfNode(*ifNode, stmtNumber);
    }
}

void DesignExtractor::updateFollows(int stmtNumber, std::vector<int>& stmtList) {
    if (!stmtList.empty()) { // 'Follows' relationship is valid only if there is a predecessor
        int prevStmtNumber = stmtList.back();
        follows[stmtNumber] = prevStmtNumber;

        // Update followsT (transitive) relationship. Since Follows is only directly preceding, we manually propagate transitive relationships.
        followsT[prevStmtNumber].insert(stmtNumber);
        for (auto& prevTransitive : followsT) {
            if (prevTransitive.second.find(prevStmtNumber) != prevTransitive.second.end()) {
                prevTransitive.second.insert(stmtNumber);
            }
        }
    }
}

void DesignExtractor::updateParent(int childStmtNumber, int parentStmtNumber) {
    if (parentStmtNumber > 0) { // Check to ensure not top-level program block
        parent[childStmtNumber] = parentStmtNumber;

        // Similar to 'Follows', we update the transitive closure of 'Parent'
        parentT[parentStmtNumber].insert(childStmtNumber);
        for (auto& prevTransitive : parentT) {
            if (prevTransitive.second.find(parentStmtNumber) != prevTransitive.second.end()) {
                prevTransitive.second.insert(childStmtNumber);
            }
        }
    }
}

void DesignExtractor::visitIfNode(const IfNode& node, int stmtNumber) {
    std::vector<int> thenStmtList, elseStmtList;
    visitBlockNode(*node.thenBranch, stmtNumber, thenStmtList); // Recursively visit 'then' branch
    if (node.elseBranch) { // If 'else' branch exists
        visitBlockNode(*node.elseBranch, stmtNumber, elseStmtList); // Recursively visit 'else' branch
    }
}