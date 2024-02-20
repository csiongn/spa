#pragma once

#include "AST.h"
#include "PKB/facade/PKBFacade.h"
#include <unordered_map>
#include <unordered_set>
#include <utility>

class DesignExtractor {
public:
    explicit DesignExtractor(std::shared_ptr<PKBFacade> pkbFacade) : pkbFacade(std::move(pkbFacade)) {}

    void extractDesign(const ProgramNode& astRoot);

    // Getters
    const std::unordered_map<int, int>& getFollows() const {
        return follows;
    }

    const std::unordered_map<int, std::unordered_set<int>>& getFollowsT() const {
        return followsT;
    }

    const std::unordered_map<int, int>& getParent() const {
        return parent;
    }

    const std::unordered_map<int, std::unordered_set<int>>& getParentT() const {
        return parentT;
    }

    const std::unordered_set<std::string>& getVariables() const {
        return variables;
    }

private:

    std::shared_ptr<PKBFacade> pkbFacade;
    std::unordered_map<int, int> follows;
    std::unordered_map<int, std::unordered_set<int>> followsT;
    std::unordered_map<int, int> parent;
    std::unordered_map<int, std::unordered_set<int>> parentT;
    std::unordered_set<std::string> variables;
    std::unordered_set<std::string> literals;
    std::unordered_set<int> assignStmts;
    std::unordered_set<int> callStmts;
    std::unordered_set<int> ifStmts;
    std::unordered_set<int> readStmts;
    std::unordered_set<int> printStmts;
    std::unordered_set<int> whileStmts;


    // Methods to traverse the AST
    void visitProgramNode(const ProgramNode& node);
    void visitProcedureNode(const ProcedureNode& node);
    void visitBlockNode(const BlockNode& node, int parentStmt, std::vector<int>& stmtList);
    void visitStmtNode(const StmtNode& node, int parentStmt, std::vector<int>& stmtList);
    void visitIfNode(const IfNode& node, int stmtNumber);
    void visitExprNode(const ExprNode& node, int stmtNumber);

    // Utility Methods
    void updateFollows(int stmtNumber, std::vector<int>& stmtList);
    void updateParent(int childStmtNumber, int parentStmtNumber);
    void insertVariable(const std::string& var);
    void insertLiteral(const std::string& var);
    void insertAssign(const int stmtNum);
    void insertCall(const int stmtNum);
    void insertIf(const int stmtNum);
    void insertRead(const int stmtNum);
    void insertPrint(const int stmtNum);
    void insertWhile(const int stmtNum);

    void pushToPKB();

    int currentStmtNumber = 0; // Keeps track of the statement numbers as we traverse
};
