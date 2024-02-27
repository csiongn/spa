#include "DesignExtractor.h"

#include <iostream>

void DesignExtractor::extractDesign(const ProgramNode& astRoot) {
    visitProgramNode(astRoot);
    pushToPKB();
}

void DesignExtractor::pushToPKB() {
    for (const auto& pair: follows) {
        pkbWriter->insertFollows(pair.second, pair.first);
    }

    for (const auto& pair: parent) {
        pkbWriter->insertParent(pair.second, pair.first);
    }

    for (const auto& pair: uses) {
        pkbWriter->insertUsesStmt(pair.first, pair.second);
    }

    for (const auto& pair: modifies) {
        pkbWriter->insertModifiesStmt(pair.first, pair.second);
    }

    for (const auto& pair: followsT) { // Iterate through the map of all followsT relationships
        for (const auto& followerStmtNum : pair.second) { // Iterate through pair.second – the set of statements that is a transitive follower of the key statement
            pkbWriter->insertFollowsT(pair.first, followerStmtNum);
        }
    }

    for (const auto& pair: parentT) { // Iterate through the map of all parentT relationships
        for (const auto& childStmtNum : pair.second) { // Iterate through pair.second – the set of statements that is a transitive child of the key statement
            pkbWriter->insertParentT(pair.first, childStmtNum);
        }
    }

    for (const auto& proc: procedures) {
        pkbWriter->insertProcedure(proc);
    }

    for (const auto& var: variables) {
        pkbWriter->insertVariable(var);
    }

    for (const auto& val: literals) {
        pkbWriter->insertConstant(std::stoi(val));
    }

    for (const auto& stmtNum: stmts) {
        pkbWriter->insertStatement(stmtNum);
    }

    for (const auto& stmtNum: assignStmts) {
        pkbWriter->insertAssign(stmtNum);
    }

    for (const auto& stmtNum: callStmts) {
        pkbWriter->insertCall(stmtNum);
    }

    for (const auto& stmtNum: ifStmts) {
        pkbWriter->insertIf(stmtNum);
    }

    for (const auto& stmtNum: readStmts) {
        pkbWriter->insertRead(stmtNum);
    }

    for (const auto& stmtNum: printStmts) {
        pkbWriter->insertPrint(stmtNum);
    }

    for (const auto& stmtNum: whileStmts) {
        pkbWriter->insertWhile(stmtNum);
    }

    for (const auto& assignNode: assignNodes) {
        pkbWriter->insertAssignPattern(assignNode->varName,
            assignNode->value->getHashValue(), assignNode->stmtNumber, assignNode->value);
    }

}


void DesignExtractor::visitProgramNode(const ProgramNode& node) {
    for (const auto& procNode : node.procedures) {
        insertProcedure(procNode->name);
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

void DesignExtractor::visitExprNode(const ExprNode& node, int stmtNumber) {
    // Binary nodes
    if (const auto* binaryExprNode = dynamic_cast<const BinaryExprNode*>(&node)) {
        // Recursively visit left and right sub-expressions
        visitExprNode(*binaryExprNode->left, stmtNumber);
        visitExprNode(*binaryExprNode->right, stmtNumber);
    }
    else if (const auto* logicalOpNode = dynamic_cast<const LogicalOpNode*>(&node)) {
        visitExprNode(*logicalOpNode->left, stmtNumber);
        visitExprNode(*logicalOpNode->right, stmtNumber);
    }
    else if (const auto* relExprNode = dynamic_cast<const RelExprNode*>(&node)) {
        visitExprNode(*relExprNode->left, stmtNumber);
        visitExprNode(*relExprNode->right, stmtNumber);
    }

    // Unary nodes (variable nodes)
    else if (const auto* varNode = dynamic_cast<const VariableNode*>(&node)) {
        updateUses(stmtNumber, varNode->name);
        insertVariable(varNode->name);
    }
    else if (const auto* literalNode = dynamic_cast<const LiteralNode*>(&node)) {
        insertLiteral(literalNode->value);
    } else if (const auto* negNode = dynamic_cast<const NegationNode*>(&node)) {
        visitExprNode(*negNode->expr, stmtNumber);
    }

}

void DesignExtractor::visitStmtNode(const StmtNode& node, int parentStmt, std::vector<int>& stmtList) {
    int stmtNumber = ++currentStmtNumber; // Assign and increment the global statement number
    updateParent(stmtNumber, parentStmt); // Update the 'Parent' relationship
    updateFollows(stmtNumber, stmtList); // Update the 'Follows' relationship
    stmtList.push_back(stmtNumber); // Add current statement to the list for potential 'Follows' relationships
    insertStmt(stmtNumber); // Insert the current statement number

    if (const auto* ifNode = dynamic_cast<const IfNode*>(&node)) {
        insertIf(stmtNumber);
        visitIfNode(*ifNode, stmtNumber);
    } else if (const auto* whileNode = dynamic_cast<const WhileNode*>(&node)) {
        insertWhile(stmtNumber);
        visitWhileNode(*whileNode, stmtNumber);
    } else if (const auto* assignNode = dynamic_cast<const AssignNode*>(&node)) {
        insertAssign(stmtNumber);
        assignNodes.insert(std::make_shared<AssignNode>(*assignNode));
        insertVariable(assignNode->varName);
        updateModifies(stmtNumber, assignNode->varName);
        visitExprNode(*assignNode->value, stmtNumber);
    } else if (const auto* callNode = dynamic_cast<const CallNode*>(&node)) {
        insertCall(stmtNumber);
    } else if (const auto* readNode = dynamic_cast<const ReadNode*>(&node)) {
        insertRead(stmtNumber);
        insertVariable(readNode->varName);
        updateModifies(stmtNumber, readNode->varName);

    } else if (const auto* printNode = dynamic_cast<const PrintNode*>(&node)) {
        insertPrint(stmtNumber);
        insertVariable(printNode->varName);
        updateUses(stmtNumber, printNode->varName);
    }
}

void DesignExtractor::visitIfNode(const IfNode& node, int stmtNumber) {
    std::vector<int> thenStmtList, elseStmtList;
    visitExprNode(*node.condition, stmtNumber);
    visitBlockNode(*node.thenBranch, stmtNumber, thenStmtList); // Recursively visit 'then' branch
    if (node.elseBranch) { // If 'else' branch exists
        visitBlockNode(*node.elseBranch, stmtNumber, elseStmtList); // Recursively visit 'else' branch
    }
}

void DesignExtractor::visitWhileNode(const WhileNode& node, int stmtNumber) {
    visitExprNode(*node.condition, stmtNumber);
    std::vector<int> stmtList;
    visitBlockNode(*node.body, stmtNumber, stmtList);
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

void DesignExtractor::updateUses(int stmtNumber, const std::string& variableName) {
    uses[stmtNumber].insert(variableName);

    int parentStmt = stmtNumber;
    while (parent.count(parentStmt)) {
        parentStmt = parent.at(parentStmt);
        uses[parentStmt].insert(variableName);
    }
}

void DesignExtractor::updateModifies(int stmtNumber, const std::string& variableName) {
    modifies[stmtNumber].insert(variableName);

    int parentStmt = stmtNumber;
    while (parent.count(parentStmt)) {
        parentStmt = parent.at(parentStmt);
        modifies[parentStmt].insert(variableName);
    }
}

void DesignExtractor::insertProcedure(const std::string& procName) {
    procedures.insert(procName);
}

void DesignExtractor::insertVariable(const std::string& var) {
    variables.insert(var);
}

void DesignExtractor::insertLiteral(const std::string& value) {
    literals.insert(value);
}

void DesignExtractor::insertStmt(const int stmtNum) {
    stmts.insert(stmtNum);
}

void DesignExtractor::insertAssign(const int stmtNum) {
    assignStmts.insert(stmtNum);
}

void DesignExtractor::insertCall(const int stmtNum) {
    callStmts.insert(stmtNum);
}

void DesignExtractor::insertIf(const int stmtNum) {
    ifStmts.insert(stmtNum);
}

void DesignExtractor::insertPrint(const int stmtNum) {
    printStmts.insert(stmtNum);
}

void DesignExtractor::insertRead(const int stmtNum) {
    readStmts.insert(stmtNum);
}

void DesignExtractor::insertWhile(const int stmtNum) {
    whileStmts.insert(stmtNum);
}
