#pragma once

#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CFG.h"
#include "ICFGVisitor.h"

class IAffectsExtractor : public ICFGVisitor {
 public:
  virtual ~IAffectsExtractor() = default;
  virtual void execute(CFGManager& cfgManager) = 0;
  virtual std::unordered_map<int, std::unordered_set<int>> getAffectsRelationships() const = 0;

  static bool isInRecursiveStructure(const CFGNode& node, CFG& cfg);
};

class IShortCircuitAffectsExtractor : public ICFGVisitor {
 public:
  virtual ~IShortCircuitAffectsExtractor() = default;
  virtual void execute(CFGManager& cfgManager) = 0;
  virtual bool getResult() = 0;

  static bool isInRecursiveStructure(const CFGNode& node, CFG& cfg);

};

class AllAffectsQuery : public IAffectsExtractor {

  void addAffects(int s1, int s2) {
    affectsRelationships[s1].insert(s2);
  };

 public:
  void visit(CFG& cfg) override;

  void execute(CFGManager& cfgManager) override {
    affectsRelationships.clear();
    for (const auto& entry : cfgManager.getCFGs()) {
      entry.second->accept(*this);
    }
  }

  std::unordered_map<int, std::unordered_set<int>> getAffectsRelationships() const override {
    return affectsRelationships;
  }

 private:
  std::unordered_map<int, std::unordered_set<int>> affectsRelationships;
};

class AffectsFromStatementQuery : public IAffectsExtractor {
 public:
  explicit AffectsFromStatementQuery(int statementNumber) : statementNumber(statementNumber) {}

  void addAffects(int s1, int s2) {
    affectsRelationships[s1].insert(s2);
  };

  void visit(CFG& cfg) override;

  void execute(CFGManager& cfgManager) override {
    auto cfg = cfgManager.getCFG(statementNumber);
    if (cfg && cfg->findNode(statementNumber)->stmtType == SimpleProgram::StatementType::ASSIGN) {
      cfg->accept(*this);
    }
  }

  std::unordered_map<int, std::unordered_set<int>> getAffectsRelationships() const override {
    return affectsRelationships;
  }

 private:
  int statementNumber;
  std::unordered_map<int, std::unordered_set<int>> affectsRelationships;
};

class AffectsFromToStatementQuery : public IShortCircuitAffectsExtractor {
 public:
  explicit AffectsFromToStatementQuery(int fromStatementNumber, int toStatementNumber) :
  fromStatementNumber(fromStatementNumber), toStatementNumber(toStatementNumber) {}

  void visit(CFG& cfg) override;

  void execute(CFGManager& cfgManager) override {
    auto cfg = cfgManager.getCFG(fromStatementNumber);
    auto cfgTo = cfgManager.getCFG(fromStatementNumber);

    if (!cfg || !cfgTo) {
      result = false;
      return;
    }

    if (cfgManager.getCFGName(fromStatementNumber) != cfgManager.getCFGName(toStatementNumber) ||
      cfgManager.getCFG(fromStatementNumber)->findNode(fromStatementNumber)->stmtType != SimpleProgram::StatementType::ASSIGN ||
      cfgManager.getCFG(toStatementNumber)->findNode(toStatementNumber)->stmtType != SimpleProgram::StatementType::ASSIGN ) {
      // Statements are not within same procedure and cannot have affect relationship and
      // Statements that are not assign statements cannot be part of affects relationships
      result = false;
      return;
    }

    cfg->accept(*this);
  }

  bool getResult() override {
    return result;
  }

 private:
  int fromStatementNumber;
  int toStatementNumber;
  bool result = false;
};

class AffectsContainsFromQuery : public IShortCircuitAffectsExtractor {
 public:
  explicit AffectsContainsFromQuery(int statementNumber) :
  statementNumber(statementNumber) {}

  void visit(CFG& cfg) override;

  void execute(CFGManager& cfgManager) override {
    auto cfg = cfgManager.getCFG(statementNumber);
    if (cfg && cfg->findNode(statementNumber)->stmtType == SimpleProgram::StatementType::ASSIGN) {
      cfg->accept(*this);
    }
  }

  bool getResult() override {
    return result;
  }

 private:
  int statementNumber;
  bool result = false;
};

class HasAffectsQuery : public IShortCircuitAffectsExtractor {
 public:
  HasAffectsQuery() {}

  void visit(CFG& cfg) override;

  void execute(CFGManager& cfgManager) override {
    for (const auto& entry : cfgManager.getCFGs()) {
      entry.second->accept(*this);
      if (result) break;
    }
  }

  bool getResult() override {
    return result;
  }

 private:
  bool result = false;
};