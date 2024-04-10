#include <string>
#include <unordered_map>
#include <unordered_set>

#include "AffectsQueryExtractor.h"

bool IAffectsExtractor::isInRecursiveStructure(const CFGNode& node, CFG& cfg) {
  std::unordered_set<int> visited;
  std::vector<int> worklist;
  worklist.push_back(node.stmtNum);

  while (!worklist.empty()) {
    int currStmtNum = worklist.back();
    worklist.pop_back();

    if (visited.count(currStmtNum) > 0) {
      continue;
    }

    visited.insert(currStmtNum);

    auto& currNode = *cfg.findNode(currStmtNum);

    if (currNode.stmtType == SimpleProgram::StatementType::WHILE && node.stmtNum < currNode.next[1]->stmtNum) {
      return true;
    }

    for (const auto& prevNode : currNode.getPrevious()) {
      if (visited.count(prevNode->stmtNum) == 0) {
        worklist.push_back(prevNode->stmtNum);
      }
    }
  }

  return false;
}

bool IShortCircuitAffectsExtractor::isInRecursiveStructure(const CFGNode& node, CFG& cfg) {
  std::unordered_set<int> visited;
  std::vector<int> worklist;
  worklist.push_back(node.stmtNum);

  while (!worklist.empty()) {
    int currStmtNum = worklist.back();
    worklist.pop_back();

    if (visited.count(currStmtNum) > 0) {
      continue;
    }

    visited.insert(currStmtNum);

    auto& currNode = *cfg.findNode(currStmtNum);

    if (currNode.stmtType == SimpleProgram::StatementType::WHILE && node.stmtNum < currNode.next[1]->stmtNum) {
      return true;
    }

    for (const auto& prevNode : currNode.getPrevious()) {
      if (visited.count(prevNode->stmtNum) == 0) {
        worklist.push_back(prevNode->stmtNum);
      }
    }
  }

  return false;
}


void AllAffectsQuery::visit(CFG& cfg) {
  std::unordered_map<int, std::unordered_map<std::string, int>> lastModifiedStmtMap;
  std::vector<int> worklist;
  worklist.push_back(cfg.startNum);

  while (!worklist.empty()) {
    int currStmtNum = worklist.back();
    worklist.pop_back();


    auto& currNode = *cfg.findNode(currStmtNum);

    // Get the last modified statements map for the current node
    auto& lastModifiedStmt = lastModifiedStmtMap[currStmtNum];

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN) {
      // Check if the current statement uses any variable
      for (const auto& var : currNode.uses) {
        if (lastModifiedStmt.count(var) <= 0) {
          continue;
        }

        int modifiedStmtNum = lastModifiedStmt[var];

        if (modifiedStmtNum != currStmtNum || isInRecursiveStructure(currNode, cfg)) {
          addAffects(modifiedStmtNum, currStmtNum);
        }
      }
    }

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN || currNode.stmtType ==
      SimpleProgram::StatementType::READ ||
      currNode.stmtType == SimpleProgram::StatementType::CALL) {
      // Update the last modified statement for each variable modified by the current statement
      for (const auto& var : currNode.modifies) {
        lastModifiedStmt[var] = currStmtNum;
      }
    }

    // Propagate the last modified statements map to the next nodes
    for (const auto& nextNode : currNode.getNext()) {
      int nextStmtNum = nextNode->stmtNum;
      auto& nextLastModifiedStmt = lastModifiedStmtMap[nextStmtNum];

      bool changed = false;
      for (const auto& pair : lastModifiedStmt) {
        const auto& var = pair.first;
        int modifiedStmtNum = pair.second;
        if (nextLastModifiedStmt.count(var) == 0 || nextLastModifiedStmt[var] < modifiedStmtNum) {
          nextLastModifiedStmt[var] = modifiedStmtNum;
          changed = true;
        }
      }

      if (changed) {
        worklist.push_back(nextStmtNum);
      }
    }
  }
}

void AffectsFromStatementQuery::visit(CFG& cfg) {
  std::unordered_map<int, std::unordered_map<std::string, int>> lastModifiedStmtMap;
  std::vector<int> worklist;
  worklist.push_back(statementNumber);

  while (!worklist.empty()) {
    int currStmtNum = worklist.back();
    worklist.pop_back();


    auto& currNode = *cfg.findNode(currStmtNum);

    // Get the last modified statements map for the current node
    auto& lastModifiedStmt = lastModifiedStmtMap[currStmtNum];

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN) {
      // Check if the current statement uses any variable
      for (const auto& var : currNode.uses) {
        if (lastModifiedStmt.count(var) <= 0) {
          continue;
        }
        int modifiedStmtNum = lastModifiedStmt[var];

        if (modifiedStmtNum != currStmtNum || isInRecursiveStructure(currNode, cfg)) {
          addAffects(modifiedStmtNum, currStmtNum);
        }
      }
    }

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN || currNode.stmtType ==
      SimpleProgram::StatementType::READ ||
      currNode.stmtType == SimpleProgram::StatementType::CALL) {
      // Update the last modified statement for each variable modified by the current statement
      for (const auto& var : currNode.modifies) {
        lastModifiedStmt[var] = currStmtNum;
      }
    }

    // Propagate the last modified statements map to the next nodes
    for (const auto& nextNode : currNode.getNext()) {
      int nextStmtNum = nextNode->stmtNum;
      auto& nextLastModifiedStmt = lastModifiedStmtMap[nextStmtNum];

      bool changed = false;
      for (const auto& pair : lastModifiedStmt) {
        const auto& var = pair.first;
        int modifiedStmtNum = pair.second;
        if (nextLastModifiedStmt.count(var) == 0 || nextLastModifiedStmt[var] < modifiedStmtNum) {
          nextLastModifiedStmt[var] = modifiedStmtNum;
          changed = true;
        }
      }

      if (changed) {
        worklist.push_back(nextStmtNum);
      }
    }
  }
}

void AffectsFromToStatementQuery::visit(CFG& cfg) {
  std::unordered_map<int, std::unordered_map<std::string, int>> lastModifiedStmtMap;
  std::vector<int> worklist;
  worklist.push_back(fromStatementNumber);

  while (!worklist.empty()) {
    int currStmtNum = worklist.back();
    worklist.pop_back();


    auto& currNode = *cfg.findNode(currStmtNum);

    // Get the last modified statements map for the current node
    auto& lastModifiedStmt = lastModifiedStmtMap[currStmtNum];

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN) {
      // Check if the current statement uses any variable
      for (const auto& var : currNode.uses) {
        if (lastModifiedStmt.count(var) <= 0) {
          continue;
        }

        int modifiedStmtNum = lastModifiedStmt[var];

        if (fromStatementNumber == modifiedStmtNum && toStatementNumber == currStmtNum &&
          (modifiedStmtNum != currStmtNum || isInRecursiveStructure(currNode, cfg))) {
          result = true;
          return;
        }
      }
    }

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN || currNode.stmtType ==
      SimpleProgram::StatementType::READ ||
      currNode.stmtType == SimpleProgram::StatementType::CALL) {
      // Update the last modified statement for each variable modified by the current statement
      for (const auto& var : currNode.modifies) {
        lastModifiedStmt[var] = currStmtNum;
      }
    }

    // Propagate the last modified statements map to the next nodes
    for (const auto& nextNode : currNode.getNext()) {
      int nextStmtNum = nextNode->stmtNum;
      auto& nextLastModifiedStmt = lastModifiedStmtMap[nextStmtNum];

      bool changed = false;
      for (const auto& pair : lastModifiedStmt) {
        const auto& var = pair.first;
        int modifiedStmtNum = pair.second;
        if (nextLastModifiedStmt.count(var) == 0 || nextLastModifiedStmt[var] < modifiedStmtNum) {
          nextLastModifiedStmt[var] = modifiedStmtNum;
          changed = true;
        }
      }

      if (changed) {
        worklist.push_back(nextStmtNum);
      }
    }
  }
}

void AffectsContainsFromQuery::visit(CFG& cfg) {
  std::unordered_map<int, std::unordered_map<std::string, int>> lastModifiedStmtMap;
  std::vector<int> worklist;
  worklist.push_back(statementNumber);

  while (!worklist.empty()) {
    int currStmtNum = worklist.back();
    worklist.pop_back();


    auto& currNode = *cfg.findNode(currStmtNum);

    // Get the last modified statements map for the current node
    auto& lastModifiedStmt = lastModifiedStmtMap[currStmtNum];

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN) {
      // Check if the current statement uses any variable
      for (const auto& var : currNode.uses) {
        if (lastModifiedStmt.count(var) <= 0) {
          continue;
        }

        int modifiedStmtNum = lastModifiedStmt[var];

        if (statementNumber == modifiedStmtNum && (modifiedStmtNum != currStmtNum || isInRecursiveStructure(
          currNode, cfg))) {
          result = true;
          return;
        }
      }
    }

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN || currNode.stmtType ==
      SimpleProgram::StatementType::READ ||
      currNode.stmtType == SimpleProgram::StatementType::CALL) {
      // Update the last modified statement for each variable modified by the current statement
      for (const auto& var : currNode.modifies) {
        lastModifiedStmt[var] = currStmtNum;
      }
    }

    // Propagate the last modified statements map to the next nodes
    for (const auto& nextNode : currNode.getNext()) {
      int nextStmtNum = nextNode->stmtNum;
      auto& nextLastModifiedStmt = lastModifiedStmtMap[nextStmtNum];

      bool changed = false;
      for (const auto& pair : lastModifiedStmt) {
        const auto& var = pair.first;
        int modifiedStmtNum = pair.second;
        if (nextLastModifiedStmt.count(var) == 0 || nextLastModifiedStmt[var] < modifiedStmtNum) {
          nextLastModifiedStmt[var] = modifiedStmtNum;
          changed = true;
        }
      }

      if (changed) {
        worklist.push_back(nextStmtNum);
      }
    }
  }
}

void HasAffectsQuery::visit(CFG& cfg) {
  std::unordered_map<int, std::unordered_map<std::string, int>> lastModifiedStmtMap;
  std::vector<int> worklist;
  worklist.push_back(cfg.startNum);

  while (!worklist.empty()) {
    int currStmtNum = worklist.back();
    worklist.pop_back();

    auto& currNode = *cfg.findNode(currStmtNum);

    // Get the last modified statements map for the current node
    auto& lastModifiedStmt = lastModifiedStmtMap[currStmtNum];

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN) {
      // Check if the current statement uses any variable
      for (const auto& var : currNode.uses) {
        if (lastModifiedStmt.count(var) > 0) {
          result = true;
          return;
        }
      }
    }

    if (currNode.stmtType == SimpleProgram::StatementType::ASSIGN || currNode.stmtType ==
      SimpleProgram::StatementType::READ ||
      currNode.stmtType == SimpleProgram::StatementType::CALL) {
      // Update the last modified statement for each variable modified by the current statement
      for (const auto& var : currNode.modifies) {
        lastModifiedStmt[var] = currStmtNum;
      }
    }

    // Propagate the last modified statements map to the next nodes
    for (const auto& nextNode : currNode.getNext()) {
      int nextStmtNum = nextNode->stmtNum;
      auto& nextLastModifiedStmt = lastModifiedStmtMap[nextStmtNum];

      bool changed = false;
      for (const auto& pair : lastModifiedStmt) {
        const auto& var = pair.first;
        int modifiedStmtNum = pair.second;
        if (nextLastModifiedStmt.count(var) == 0 || nextLastModifiedStmt[var] < modifiedStmtNum) {
          nextLastModifiedStmt[var] = modifiedStmtNum;
          changed = true;
        }
      }

      if (changed) {
        worklist.push_back(nextStmtNum);
      }
    }
  }
}
