#include <vector>
#include <unordered_set>
#include <utility>

#include "control_flow_graph/AffectsQueryExtractor.h"
#include "RuntimeExtractor.h"

bool RuntimeExtractor::hasNextT() {
  return nextExtractor->hasNextT();
}

bool RuntimeExtractor::containsNextT(int stmtNoFrom, int stmtNoTo) {
  return nextExtractor->containsNextT(stmtNoFrom, stmtNoTo);
}

bool RuntimeExtractor::containsNextTFrom(int stmtNo) {
  return nextExtractor->containsNextTFrom(stmtNo);
}

bool RuntimeExtractor::containsNextTTo(int stmtNo) {
  return nextExtractor->containsNextTTo(stmtNo);
}

std::vector<int> RuntimeExtractor::getStatementsNextTFrom(int stmtNo) {
  return nextExtractor->getStatementsNextTFrom(stmtNo);
}

std::vector<int> RuntimeExtractor::getStatementsNextTTo(int stmtNo) {
  return nextExtractor->getStatementsNextTTo(stmtNo);
}

std::vector<int> RuntimeExtractor::getStatementsNextTFrom() {
  return nextExtractor->getStatementsNextTFrom();
}

std::vector<int> RuntimeExtractor::getStatementsNextTTo() {
  return nextExtractor->getStatementsNextTTo();
}

std::vector<std::pair<int, int>> RuntimeExtractor::getNextT() {
  return nextExtractor->getNextT();
}

bool RuntimeExtractor::hasAffects() {
  std::unique_ptr<IShortCircuitAffectsExtractor> query = \
    std::make_unique<HasAffectsQuery>();

  query->execute(*cfgManager);

  return query->getResult();
}

bool RuntimeExtractor::containsAffects(int stmtNoFrom, int stmtNoTo) {
  std::unique_ptr<IShortCircuitAffectsExtractor> query = \
    std::make_unique<AffectsFromToStatementQuery>(stmtNoFrom, stmtNoTo);

  query->execute(*cfgManager);

  return query->getResult();
}

bool RuntimeExtractor::containsAffectsFrom(int stmtNo) {
  std::unique_ptr<IShortCircuitAffectsExtractor> query = \
    std::make_unique<AffectsContainsFromQuery>(stmtNo);

  query->execute(*cfgManager);

  return query->getResult();
}

bool RuntimeExtractor::containsAffectsTo(int stmtNo) {
  std::unique_ptr<IAffectsExtractor> query = std::make_unique<AllAffectsQuery>();
  query->execute(*cfgManager);

  // Convert std::unordered_set to std::vector because QPS requires it as std::vector
  auto setResults = query->getAffectsRelationships();
  std::vector<int> arrResults;
  for (auto pair : setResults) {
    if (pair.second.count(stmtNo)) {
      return true;
    }
  }

  return false;
}

std::vector<int> RuntimeExtractor::getStatementsAffectsFrom(int stmtNo) {
  std::unique_ptr<IAffectsExtractor> query = std::make_unique<AffectsFromStatementQuery>(stmtNo);
  query->execute(*cfgManager);

  // Convert std::unordered_set to std::vector because QPS requires it as std::vector
  auto setResults = query->getAffectsRelationships();
  std::vector<int> arrResults;
  for (int affectedStmtNum : setResults[stmtNo]) {
      arrResults.emplace_back(affectedStmtNum);
  }

  return arrResults;
}

std::vector<int> RuntimeExtractor::getStatementsAffectsTo(int stmtNo) {
  std::unique_ptr<IAffectsExtractor> query = std::make_unique<AllAffectsQuery>();
  query->execute(*cfgManager);

  // Convert std::unordered_set to std::vector because QPS requires it as std::vector
  auto setResults = query->getAffectsRelationships();
  std::vector<int> arrResults;
  for (auto pair : setResults) {
    if (pair.second.count(stmtNo)) {
      arrResults.emplace_back(pair.first);
    }
  }

  return arrResults;
}

std::vector<int> RuntimeExtractor::getStatementsAffectsFrom() {
  std::unique_ptr<IAffectsExtractor> query = std::make_unique<AllAffectsQuery>();
  query->execute(*cfgManager);

  auto allResults = query->getAffectsRelationships();
  std::unordered_set<int> setResults;

  for (const auto& pair : allResults) {
    setResults.insert(pair.first);
  }

  // Convert std::unordered_set to std::vector because QPS requires it as std::vector
  std::vector<int> arrResults;
  for (int stmtNo : setResults) {
    arrResults.emplace_back(stmtNo);
  }

  return arrResults;
}

std::vector<int> RuntimeExtractor::getStatementsAffectsTo() {
  std::unique_ptr<IAffectsExtractor> query = std::make_unique<AllAffectsQuery>();
  query->execute(*cfgManager);

  auto allResults = query->getAffectsRelationships();
  std::unordered_set<int> setResults;

  for (const auto& pair : allResults) {
    for (const int stmtNo : pair.second) {
      setResults.insert(stmtNo);
    }
  }

  // Convert std::unordered_set to std::vector because QPS requires it as std::vector
  std::vector<int> arrResults;
  for (int stmtNo : setResults) {
    arrResults.emplace_back(stmtNo);
  }

  return arrResults;
}

std::vector<std::pair<int, int>> RuntimeExtractor::getAffects() {
  std::unique_ptr<IAffectsExtractor> query = std::make_unique<AllAffectsQuery>();
  query->execute(*cfgManager);

  // Convert std::unordered_set to std::vector because QPS requires it as std::vector
  auto setResults = query->getAffectsRelationships();
  std::vector<std::pair<int, int>> arrResults;
  for (auto pair : setResults) {
    for (int affectedStmtNum : pair.second) {
      arrResults.emplace_back(pair.first, affectedStmtNum);
    }
  }

  return arrResults;
}
