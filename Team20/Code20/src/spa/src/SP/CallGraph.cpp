#include <bitset>
#include <iostream>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CallGraph.h"
#include "PKB/facade/IPKBWriter.h"

void CallGraph::computeTransitiveCall() {
  /* Single threaded implementation
  for (int x = 0; x < index; x++) {
    for (int y = 0; y < index; y++) {
      for (int z = 0; z < index; z++) {
        adjMatrix[x][y] = adjMatrix[x][y] || (adjMatrix[x][z] && adjMatrix[z][y]);
      }
    }
  } */
  const int numThreads = std::thread::hardware_concurrency();
  std::vector<std::future<void>> futures;

  for (int k = 0; k < index; ++k) {
    auto updateMatrix = [this, k](int start, int end) {
      for (int i = start; i < end; ++i) {
        for (int j = 0; j < index; ++j) {
          adjMatrix[i][j] = adjMatrix[i][j] || (adjMatrix[i][k] && adjMatrix[k][j]);
        }
      }
    };

    // Divide rows among tasks for parallel processing
    int rowsPerTask = index / numThreads;
    for (int t = 0; t < numThreads; ++t) {
      int start = t * rowsPerTask;
      int end = (t + 1) * rowsPerTask;
      if (t == numThreads - 1) {
        end = index; // Handle any remaining rows in the last task
      }
      futures.emplace_back(std::async(std::launch::async, updateMatrix, start, end));
    }

    // Wait for all tasks to complete for this k
    for (auto& f : futures) {
      f.get(); // This blocks until the future is ready
    }
    futures.clear(); // Clear the futures for the next iteration
  }
}

// Adds call relationship into the call graph stored as adjacency list
void CallGraph::addCall(const std::string& caller, const std::string& called) {
  if (finalized) {
    throw std::runtime_error("Tried to add call relationship after call graph is finalized.");
  }
  if (!nameToIndex.count(caller)) {
    adjLst.emplace_back();
    nameToIndex[caller] = index;
    indexToName.push_back(caller);
    index++;
  }

  if (!nameToIndex.count(called)) {
    adjLst.emplace_back();
    nameToIndex[called] = index;
    indexToName.push_back(called);
    index++;
  }

  int callerIndex = nameToIndex[caller];
  int calledIndex = nameToIndex[called];

  adjLst.at(callerIndex).insert(calledIndex);
}

// Convert adjacency list to adjacency matrix and compute transitive call relationships
void CallGraph::finalize() {
  if (finalized) {
    throw std::runtime_error("Tried to finalize call graph after call graph is finalized.");
  }

  adjMatrix.resize(index, std::bitset<MAX_NODES>());

  for (int m = 0; m < index; m++) {
    for (int n : adjLst[m]) {
      adjMatrix[m][n] = true;
    }
  }

  // Use Floyd-Warshall to compute transitive call relationships
  computeTransitiveCall();
  finalized = true;
}

void CallGraph::pushToPKB(const std::shared_ptr<IPKBWriter>& pkbWriter) {
  if (!finalized) {
    throw std::runtime_error("Tried to push calls relationships to PKB without finalizing call graph.");
  }

  // Push Calls relationships
  for (int callingProc = 0; callingProc < index; callingProc++) {
    for (int calledProc : adjLst[callingProc]) {
      pkbWriter->insertCallsProc(indexToName[callingProc], indexToName[calledProc]);
    }
  }

  // Push CallsT relationships
  for (int callingProc = 0; callingProc < index; callingProc++) {
    for (int calledProc = 0; calledProc < index; calledProc++) {
      if (adjMatrix[callingProc][calledProc]) {
        pkbWriter->insertCallsTProc(indexToName[callingProc], indexToName[calledProc]);
      }
    }
  }
}

void CallGraph::pushToPKB(const std::shared_ptr<IPKBWriter>& pkbWriter, std::unordered_map<std::string, std::unordered_set<std::string>> procsUses, std::unordered_map<std::string, std::unordered_set<std::string>> procsModifies) {
  if (!finalized) {
    throw std::runtime_error("Tried to push calls relationships to PKB without finalizing call graph.");
  }

  // Push Calls relationships
  for (int callingProc = 0; callingProc < index; callingProc++) {
    for (int calledProc : adjLst[callingProc]) {
      pkbWriter->insertCallsProc(indexToName[callingProc], indexToName[calledProc]);
    }
  }

  // Push CallsT relationships
  for (int callingProc = 0; callingProc < index; callingProc++) {
    for (int calledProc = 0; calledProc < index; calledProc++) {
      if (adjMatrix[callingProc][calledProc]) {
        pkbWriter->insertCallsTProc(indexToName[callingProc], indexToName[calledProc]);
        for (auto& varName : procsUses[indexToName[calledProc]]) {
          pkbWriter->insertUsesProc(indexToName[callingProc], varName);
        }

        for (auto& varName : procsModifies[indexToName[calledProc]]) {
          pkbWriter->insertModifiesProc(indexToName[callingProc], varName);
        }
      }
    }
  }
}


bool CallGraph::hasCallsRelationship(const std::string& a, const std::string& b) {
  if (!nameToIndex.count(a) || !nameToIndex.count(b)) {
    return false;
  }
  return adjLst[nameToIndex[a]].count(nameToIndex[b]);
}

bool CallGraph::hasCallsTRelationship(const std::string& a, const std::string& b) {
  if (!finalized) {
    throw std::runtime_error("Tried to retrieve CallsT relationship without finalizing call graph");
  }

  if (!nameToIndex.count(a) || !nameToIndex.count(b)) {
    return false;
  }

  return adjMatrix[nameToIndex[a]][nameToIndex[b]];
}
