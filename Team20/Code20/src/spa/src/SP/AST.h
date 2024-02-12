#pragma once
#include <string>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;

    // Convert AST rooted at ASTNode into a string using custom convention

    /* Convention:
     * Program [<ProcedureNodes>]
     * Procedure [<BlockNode>]
     *
     * 1. For Statement nodes (StmtNode)
     * Block [<StmtNodes>]
     * Assign varName [<ExprNode>]
     * Call procName
     * Read varName
     * Print varName
     * While [<ExprNode for condition>] [<BlockNode>]
     * If [<ExprNode>] then [<BlockNode>] else [<BlockNode>]
     *
     * 2. For expression nodes (ExprNode)
     * BinaryExpr [<ExprNode> op <ExprNode>]
     * Literal val
     * Variable varName
     */

    virtual std::string serialize() const = 0;
};

class ExprNode : public ASTNode {};
class StmtNode : public ASTNode {};

// BlockNode for statement lists
class BlockNode : public ASTNode {
public:
    std::vector<std::unique_ptr<StmtNode> > statements;

    BlockNode(std::vector<std::unique_ptr<StmtNode> > statements)
        : statements(std::move(statements)) {}

    std::string serialize() const override {
        std::string result = "Block [";
        for (const auto& stmt : statements) {
            result += stmt->serialize() + " ";
        }
        result.pop_back(); // Remove the last space
        result += "]";
        return result;
    }
};

class ProcedureNode : public ASTNode {
public:
    std::string name;
    std::unique_ptr<BlockNode> body;

    ProcedureNode(std::string name, std::unique_ptr<BlockNode> body)
        : name(std::move(name)), body(std::move(body)) {}

    std::string serialize() const override {
        return "Procedure " + name + " [" + body->serialize() + "]";
    }
};

class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ProcedureNode>> procedures;

    explicit ProgramNode(std::vector<std::unique_ptr<ProcedureNode>> procedures)
        : procedures(std::move(procedures)) {}

    std::string serialize() const override {
        std::string result = "Program [";
        for (const auto& proc : procedures) {
            result += proc->serialize() + " ";
        }
        result.pop_back(); // Remove the last space
        result += "]";
        return result;
    }
};

class BinaryExprNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    std::string op;

    BinaryExprNode(std::unique_ptr<ExprNode> left, std::string op, std::unique_ptr<ExprNode> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::string serialize() const override {
        return "BinaryExpr [" + left->serialize() + " " + op + " " + right->serialize() + "]";
    }

};

class NegationNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> expr;

    explicit NegationNode(std::unique_ptr<ExprNode> expr) : expr(std::move(expr)) {}

    std::string serialize() const override {
        return "Not [" + expr->serialize() + "]";
    }
};

class LogicalOpNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    std::string op;

    LogicalOpNode(std::unique_ptr<ExprNode> left, std::string op, std::unique_ptr<ExprNode> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::string serialize() const override {
        return "LogicalOp [" + left->serialize() + " " + op + " " + right->serialize() + "]";
    }
};

class RelExprNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    std::string relational_op; // Relational operators are >, <, ==, !=, >=, <=

    RelExprNode(std::unique_ptr<ExprNode> left, std::string relational_op, std::unique_ptr<ExprNode> right)
        : left(std::move(left)), relational_op(std::move(relational_op)), right(std::move(right)) {}

    std::string serialize() const override {
        return "RelExpr [" + left->serialize() + " " + relational_op + " " + right->serialize() + "]";
    }
};

class LiteralNode : public ExprNode {
public:
    std::string value;

    explicit LiteralNode(std::string value) : value(std::move(value)) {}

    std::string serialize() const override {
        return "Literal " + value;
    }
};

class VariableNode : public ExprNode {
public:
    std::string name;

    explicit VariableNode(std::string name) : name(std::move(name)) {}

    std::string serialize() const override {
        return "Variable " + name;
    }
};

class AssignNode : public StmtNode {
public:
    std::string varName;
    std::unique_ptr<ExprNode> value;

    AssignNode(std::string varName, std::unique_ptr<ExprNode> value)
        : varName(std::move(varName)), value(std::move(value)) {}

    std::string serialize() const override {
        return "Assign " + varName + " [" + value->serialize() + "]";
    }
};

class CallNode : public StmtNode {
public:
    std::string procName;

    explicit CallNode(std::string procName) : procName(std::move(procName)) {}

    std::string serialize() const override {
        return "Call " + procName;
    }
};

class ReadNode : public StmtNode {
public:
    std::string varName;

    explicit ReadNode(std::string varName) : varName(std::move(varName)) {}

    std::string serialize() const override {
        return "Read " + varName;
    }
};

class PrintNode : public StmtNode {
public:
    std::string varName;

    explicit PrintNode(std::string varName) : varName(std::move(varName)) {}

    std::string serialize() const override {
        return "Print " + varName;
    }
};

class WhileNode : public StmtNode {
public:
    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<BlockNode> body;

    WhileNode(std::unique_ptr<ExprNode> condition, std::unique_ptr<BlockNode> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    std::string serialize() const override {
        return "While [" + condition->serialize() + "] [" + body->serialize() + "]";
    }
};

class IfNode : public StmtNode {
public:
    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<BlockNode> thenBranch;
    std::unique_ptr<BlockNode> elseBranch; // Can be null if no else branch is present

    IfNode(std::unique_ptr<ExprNode> condition,
           std::unique_ptr<BlockNode> thenBranch,
           std::unique_ptr<BlockNode> elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}

    std::string serialize() const override {
        std::string serialized = "If [" + condition->serialize() + "] then [" + thenBranch->serialize() + "]";
        if (elseBranch) {
            serialized += " else [" + elseBranch->serialize() + "]";
        }
        return serialized;
    }
};

inline std::string serializeAST(const ASTNode* astRoot) {
    if (astRoot == nullptr) {
        return "Empty AST";
    }
    return astRoot->serialize();
}
