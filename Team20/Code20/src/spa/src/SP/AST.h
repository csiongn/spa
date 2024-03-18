#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 0. Abstract Base Node
class ASTNode {
 public:
  // Abstract Methods
  virtual std::string serialize() const = 0;
  // Destructor
  virtual ~ASTNode() = default;
};

// 1. Expression Nodes

// 1a. Abstract Expression Nodes

class ExprNode : virtual public ASTNode {
 public:
  // Abstract Methods
  virtual size_t getHashValue() const = 0;
  virtual bool containsSubtree(const ExprNode &subtree) const = 0;

  // Destructor
  virtual ~ExprNode() = default;
};

class UnaryNode : virtual public ExprNode {
 public:
  // Attributes
  std::string value;
  // Golden Ratio constant used for better hash scattering
  // See https://softwareengineering.stackexchange.com/a/402543
  static const size_t GOLDEN_RATIO_CONSTANT = 0x9e3779b1;
  size_t hashValue = std::numeric_limits<size_t>::max();

  // Constructor
  explicit UnaryNode(std::string value) : value(std::move(value)) {
	initializeHash();
  }

  // Methods
  void initializeHash() {
	std::hash<std::string> hasher;
	size_t result = hasher(value);

	hashValue = result;
  }

  size_t getHashValue() const override {
	return this->hashValue;
  };

  bool containsSubtree(const ExprNode &subtree) const override {
	const auto *unaryNode = dynamic_cast<const UnaryNode *>(&subtree);

	return unaryNode && this->hashValue == unaryNode->hashValue; // Check that subtree is a UnaryNode and hash matches
  }

  // Destructor
  virtual ~UnaryNode() = default;
};

class BinaryNode : virtual public ExprNode {
 public:
  // Attributes
  std::shared_ptr<ExprNode> left;
  std::shared_ptr<ExprNode> right;
  std::string op;
  // Golden Ratio constant used for better hash scattering
  // See https://softwareengineering.stackexchange.com/a/402543
  static const size_t GOLDEN_RATIO_PRIME = 0x9e3779b1;
  size_t hashValue = std::numeric_limits<size_t>::max();

  // Constructor
  BinaryNode(std::shared_ptr<ExprNode> left, std::string op, std::shared_ptr<ExprNode> right)
	  : left(std::move(left)), op(std::move(op)), right(std::move(right)) {
	initializeHash();
  }

  // Methods
  void initializeHash() {
	std::hash<std::string> hasher;
	size_t result = hasher(op);
	// No need to recursively computeHash since already computed at constructor
	if (this->left != nullptr) {
	  result ^= left->getHashValue() + GOLDEN_RATIO_PRIME + (result << 6) + (result >> 2);
	}
	if (this->right != nullptr) {
	  result ^= this->right->getHashValue() + GOLDEN_RATIO_PRIME + (result << 6) * (result >> 2);
	}

	hashValue = result;
  }

  size_t getHashValue() const override {
	return this->hashValue;
  };

  // if is the same tree, will return true as well
  bool containsSubtree(const ExprNode &subtree) const override {
	const auto *binaryNode = dynamic_cast<const BinaryNode *>(&subtree);
	if (!binaryNode) {
	  const auto *unaryNode = dynamic_cast<const UnaryNode *>(&subtree);
	  // NegationNode is not a UnaryNode and BinaryNode and can theoretically be a possible argument.
	  // But, NegationNode is only used in conditions and we are not checking subtrees in conditoins. Hence,
	  // we can make the assertion below.
	  assert(unaryNode);
	  return this->left->containsSubtree(*unaryNode) || this->right->containsSubtree(*unaryNode);
	}

	return this->hashValue == binaryNode->hashValue ||
		this->left && this->left->containsSubtree(subtree) ||
		this->right && this->right->containsSubtree(subtree);
  }

  // Destructor
  virtual ~BinaryNode() = default;
};

// 1b. Concrete Expression Nodes

class NegationNode : public ExprNode {
 private:
  // Methods
  void initializeHash() {
	std::hash<size_t> hasher;
	size_t result = hasher(GOLDEN_RATIO);
	result ^= expr->getHashValue() + GOLDEN_RATIO_PRIME + (result << 6) + (result >> 2);
	hashValue = result;
  }

 public:
  // Attributes
  std::shared_ptr<ExprNode> expr;
  static const size_t GOLDEN_RATIO = 0x9e3779b9;
  static const size_t GOLDEN_RATIO_PRIME = 0x9e3779b1;
  size_t hashValue;

  // Constructor
  explicit NegationNode(std::shared_ptr<ExprNode> expr) : expr(std::move(expr)) {
	initializeHash();
  }

  // Methods
  size_t getHashValue() const override {
	return this->hashValue;
  }

  bool containsSubtree(const ExprNode &subtree) const override {
	const auto *negationNode = dynamic_cast<const NegationNode *>(&subtree);
	if (!negationNode) {
	  return expr->containsSubtree(subtree);
	}

	return this->hashValue == negationNode->getHashValue() || expr->containsSubtree(subtree);
  }

  std::string serialize() const override {
	return "Not [" + expr->serialize() + "]";
  }
};

class BinaryExprNode : public BinaryNode {
 public:
  // Constructor
  BinaryExprNode(std::shared_ptr<ExprNode> left, std::string op, std::shared_ptr<ExprNode> right)
	  : BinaryNode(std::move(left), std::move(op), std::move(right)) {}

  // Methods
  std::string serialize() const override {
	return "BinaryExpr [" + left->serialize() + " " + op + " " + right->serialize() + "]";
  }
};

class LogicalOpNode : public BinaryNode {
 public:
  // Constructor
  LogicalOpNode(std::shared_ptr<ExprNode> left, std::string op, std::shared_ptr<ExprNode> right)
	  : BinaryNode(std::move(left), std::move(op), std::move(right)) {}

  // Methods
  std::string serialize() const override {
	return "LogicalOp [" + left->serialize() + " " + op + " " + right->serialize() + "]";
  }
};

class RelExprNode : public BinaryNode {
 public:
  // Constructor
  RelExprNode(std::shared_ptr<ExprNode> left, std::string op, std::shared_ptr<ExprNode> right)
	  : BinaryNode(std::move(left), std::move(op), std::move(right)) {}

  // Methods
  std::string serialize() const override {
	return "RelExpr [" + left->serialize() + " " + op + " " + right->serialize() + "]";
  }
};

class LiteralNode : public UnaryNode {
 public:
  // Constructor
  explicit LiteralNode(std::string value) : UnaryNode(std::move(value)) {}

  // Methods
  std::string serialize() const override {
	return "Literal " + value;
  }
};

class VariableNode : public UnaryNode {
 public:
  // Constructor
  explicit VariableNode(std::string name) : UnaryNode(std::move(name)) {}

  // Methods
  std::string serialize() const override {
	return "Variable " + value;
  }
};

// 2. Statement Nodes

// 2a. Abstract Statement Nodes
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
	for (const auto &stmt : statements) {
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
	for (const auto &proc : procedures) {
	  result += proc->serialize() + " ";
	}
	result.pop_back(); // Remove the last space
	result += "]";
	return result;
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
	return "Call-" + std::to_string(this->stmtNumber) + " " + procName;
  }
};

class ReadNode : public StmtNode {
 public:
  std::string varName;

  ReadNode(uint16_t stmtNumber, std::string varName)
	  : StmtNode(stmtNumber), varName(std::move(varName)) {}

  std::string serialize() const override {
	return "Read-" + std::to_string(this->stmtNumber) + " " + varName;
  }
};

class PrintNode : public StmtNode {
 public:
  std::string varName;

  explicit PrintNode(uint16_t stmtNumber, std::string varName)
	  : StmtNode(stmtNumber), varName(std::move(varName)) {}

  std::string serialize() const override {
	return "Print-" + std::to_string(this->stmtNumber) + " " + varName;
  }
};

class WhileNode : public StmtNode {
 public:
  std::shared_ptr<ExprNode> condition;
  std::shared_ptr<BlockNode> body;

  WhileNode(uint16_t stmtNumber, std::shared_ptr<ExprNode> condition, std::shared_ptr<BlockNode> body)
	  : StmtNode(stmtNumber), condition(std::move(condition)), body(std::move(body)) {}

  std::string serialize() const override {
	return "While-" + std::to_string(this->stmtNumber) + " [" + condition->serialize() + "] [" + body->serialize()
		+ "]";
  }
};

class IfNode : public StmtNode {
 public:
  std::shared_ptr<ExprNode> condition;
  std::shared_ptr<BlockNode> thenBranch;
  std::shared_ptr<BlockNode> elseBranch;

  IfNode(uint16_t stmtNumber,
		 std::shared_ptr<ExprNode> condition,
		 std::shared_ptr<BlockNode> thenBranch,
		 std::shared_ptr<BlockNode> elseBranch)
	  : StmtNode(stmtNumber),
		condition(std::move(condition)),
		thenBranch(std::move(thenBranch)),
		elseBranch(std::move(elseBranch)) {}

  std::string serialize() const override {
	std::string serialized =
		"If-" + std::to_string(this->stmtNumber) + " [" + condition->serialize() + "] then [" + thenBranch->serialize()
			+ "]";
	if (elseBranch) {
	  serialized += " else [" + elseBranch->serialize() + "]";
	}
	return serialized;
  }
};

inline std::string serializeAST(const ASTNode *astRoot) {
  if (astRoot == nullptr) {
	return "Empty AST";
  }
  return astRoot->serialize();
}
