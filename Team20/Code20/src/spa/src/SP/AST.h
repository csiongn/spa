#pragma once

#include <iostream>
#include <memory>
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

class ExprNode : public ASTNode {
public:

    std::shared_ptr<ExprNode> left;
    std::shared_ptr<ExprNode> right;
    // Golden Ratio constant used for better hash scattering
    // See https://softwareengineering.stackexchange.com/a/402543
    static const size_t GOLDEN_RATIO_CONSTANT = 0x9e3779b1;
    std::string value;

    size_t hashValue = std::numeric_limits<size_t>::max();

    size_t computeHash() {
        // empty string value not handled, not sure how

        if (this->hashValue != std::numeric_limits<size_t>::max()) {
            return this->hashValue;
        }
        std::hash<std::string> hasher;
        size_t result = hasher(value);
        // No need to recursively computeHash since already computed at constructor
        if (this->left != nullptr) {
            result ^= left->getHashValue() + GOLDEN_RATIO_CONSTANT + (result << 6) + (result >> 2);
        }
        if (this->right != nullptr) {
            result ^= this->right->getHashValue() + GOLDEN_RATIO_CONSTANT + (result << 6) * (result >> 2);
        }

        setHashValue(result);
        return result;
    }
//public:
    bool isLeaf() {
        return this->left == nullptr && this->right == nullptr;
    }
    // Do we need these getter methods
    std::shared_ptr<ExprNode> getLeft() const {
        return this->left;
    }

    void setLeft(const std::shared_ptr<ExprNode>& leftNode) {
        this->left = leftNode;
    }

    std::shared_ptr<ExprNode> getRight() const {
        return this->right;
    }

    void setRight(const std::shared_ptr<ExprNode>& rightNode) {
        this->right = rightNode;
    }

    void setHashValue(size_t value) {
        this->hashValue = value;
    }

    size_t getHashValue() const {
        return this->hashValue;
    }

    void setValue(std::string value) {
        this->value = value;
    }

    std::string getValue() {
        return this->value;
    }

    // override ==
    bool operator==(const ExprNode& other) const {
        return this->getHashValue() == other.getHashValue();
    }

    // print method, or override << later
    void print() const {
        std::cout << "Node: " << value << "- Hash: " << getHashValue() << std::endl;
    }

    // if is the same tree, will return true as well
    bool containsSubtree(const ExprNode& subtree) const {
        if (this->operator==(subtree)) {
            return true;
        }
        if (this->left != nullptr && this->left->containsSubtree(subtree)) {
            return true;
        }
        if (this->right != nullptr && this->right->containsSubtree(subtree)) {
            return true;
        }
        return false;
    }
};



class StmtNode : public ASTNode {
public:
    uint16_t stmtNumber;

    // Constructor
    explicit StmtNode(uint16_t stmtNumber) : stmtNumber(stmtNumber) {}

};

// BlockNode for statement lists
class BlockNode : public ASTNode {
public:
    std::vector<std::shared_ptr<StmtNode> > statements;

    BlockNode(std::vector<std::shared_ptr<StmtNode> > statements)
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
    std::shared_ptr<BlockNode> body;

    ProcedureNode(std::string name, std::shared_ptr<BlockNode> body)
        : name(std::move(name)), body(std::move(body)) {}

    std::string serialize() const override {
        return "Procedure " + name + " [" + body->serialize() + "]";
    }
};

class ProgramNode : public ASTNode {
public:
    std::vector<std::shared_ptr<ProcedureNode>> procedures;

    explicit ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures)
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
    //std::shared_ptr<ExprNode> left;
    //std::shared_ptr<ExprNode> right;
    std::string op;

    BinaryExprNode(std::shared_ptr<ExprNode> left, std::string op, std::shared_ptr<ExprNode> right): op(std::move(op)) {
        setLeft(left);
        setRight(right);
        setValue(this->op);
        // can only call computeHash() in derived classes, to ensure that value in ExprNode is set properly
        computeHash();
    }

    //: left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::string serialize() const override {
        return "BinaryExpr [" + left->serialize() + " " + op + " " + right->serialize() + "]";
    }

};

class NegationNode : public ExprNode {
public:
    std::shared_ptr<ExprNode> expr;

    explicit NegationNode(std::shared_ptr<ExprNode> expr) : expr(std::move(expr)) {}

    std::string serialize() const override {
        return "Not [" + expr->serialize() + "]";
    }
};

class LogicalOpNode : public ExprNode {
public:
    //std::shared_ptr<ExprNode> left;
    //std::shared_ptr<ExprNode> right;
    std::string op;

    LogicalOpNode(std::shared_ptr<ExprNode> left, std::string op, std::shared_ptr<ExprNode> right)
        : op(std::move(op)) {
        setLeft(left);
        setRight(right);
        setValue(this->op);
    }

    std::string serialize() const override {
        return "LogicalOp [" + left->serialize() + " " + op + " " + right->serialize() + "]";
    }
};

class RelExprNode : public ExprNode {
public:
    //std::shared_ptr<ExprNode> left;
    //std::shared_ptr<ExprNode> right;
    std::string relational_op; // Relational operators are >, <, ==, !=, >=, <=

    RelExprNode(std::shared_ptr<ExprNode> left, std::string relational_op, std::shared_ptr<ExprNode> right)
        : relational_op(std::move(relational_op)) {
        setLeft(left);
        setRight(right);
        setValue(this->relational_op);
        computeHash();
    }

    std::string serialize() const override {
        return "RelExpr [" + left->serialize() + " " + relational_op + " " + right->serialize() + "]";
    }
};

class LiteralNode : public ExprNode {
public:
    std::string literalValue;

    explicit LiteralNode(std::string value) : literalValue(std::move(value)) {
        setValue(literalValue);
        computeHash();
    }

    std::string serialize() const override {
        return "Literal " + literalValue;
    }
};

class VariableNode : public ExprNode {
public:
    std::string name;

    explicit VariableNode(std::string name) : name(std::move(name)) {
        setValue(this->name);
        computeHash();
    }

    std::string serialize() const override {
        return "Variable " + name;
    }
};

class AssignNode : public StmtNode {
public:
    std::string varName;
    std::shared_ptr<ExprNode> value;

    AssignNode(uint16_t stmtNumber, std::string varName, std::shared_ptr<ExprNode> value)
        : StmtNode(stmtNumber), varName(std::move(varName)), value(std::move(value)) {}

    std::string serialize() const override {
        return "Assign-" + std::to_string(this->stmtNumber) + " " + varName + " [" + value->serialize() + "]";
    }
};

class CallNode : public StmtNode {
public:
    std::string procName;

    CallNode(uint16_t stmtNumber, std::string procName)
        : StmtNode(stmtNumber), procName(std::move(procName)) {}

    std::string serialize() const override {
        return "Call-" + std::to_string(this->stmtNumber) +" " + procName;
    }
};

class ReadNode : public StmtNode {
public:
    std::string varName;

    ReadNode(uint16_t stmtNumber, std::string varName)
        : StmtNode(stmtNumber), varName(std::move(varName)) {}

    std::string serialize() const override {
        return "Read-" + std::to_string(this->stmtNumber) +" " + varName;
    }
};

class PrintNode : public StmtNode {
public:
    std::string varName;

    explicit PrintNode(uint16_t stmtNumber, std::string varName)
        : StmtNode(stmtNumber), varName(std::move(varName)) {}

    std::string serialize() const override {
        return "Print-" + std::to_string(this->stmtNumber) +" " + varName;
    }
};

class WhileNode : public StmtNode {
public:
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<BlockNode> body;

    WhileNode(uint16_t stmtNumber, std::shared_ptr<ExprNode> condition, std::shared_ptr<BlockNode> body)
        : StmtNode(stmtNumber), condition(std::move(condition)), body(std::move(body)) {}

    std::string serialize() const override {
        return "While-" + std::to_string(this->stmtNumber) +" [" + condition->serialize() + "] [" + body->serialize() + "]";
    }
};

class IfNode : public StmtNode {
public:
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<BlockNode> thenBranch;
    std::shared_ptr<BlockNode> elseBranch; // Can be null if no else branch is present

    IfNode(uint16_t stmtNumber,
           std::shared_ptr<ExprNode> condition,
           std::shared_ptr<BlockNode> thenBranch,
           std::shared_ptr<BlockNode> elseBranch)
        : StmtNode(stmtNumber),
          condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}

    std::string serialize() const override {
        std::string serialized = "If-" + std::to_string(this->stmtNumber) + " [" + condition->serialize() + "] then [" + thenBranch->serialize() + "]";
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
