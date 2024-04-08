#include <algorithm>
#include <future>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "NextExtractor.h"
#include "ICFGVisitor.h"
#include "CFG.h"

void NextExtractor::visit(const std::shared_ptr<CFGNode>& node) {
    if (visited.insert(node->stmtNum).second) { // Insert returns a pair, .second is true if the element was inserted
        for (const auto& nextNode : node->getNext()) {
            nextRelationships[node->stmtNum].push_back(nextNode->stmtNum);
            nextNode->accept(*this); // Recursively visit next nodes
        }
    }
}

const std::unordered_map<int, std::vector<int>>& NextExtractor::getNextRelationships() const {
    return nextRelationships;
}

void NextExtractor::pushToPKB(const std::shared_ptr<IPKBWriter> &pkbWriter) {
    for (const auto pair : nextRelationships) {
        for (const auto nextStmt : pair.second) {
            pkbWriter->insertNext(pair.first, nextStmt);
        }
    }
}

// Converts representation of Next relationships from adjacency list to an adjacency matrix
void NextExtractor::finalize() {
    if (finalized) {
        throw std::runtime_error("Tried to finalize next extractor data structures after it is finalized.");
    }

    // Compute reverse next graph
    for (const auto& pair : nextRelationships) {
        int from = pair.first;
        for (int to : pair.second) {
            revNextRelationships[to].push_back(from);
        }
    }

    // Compute next graph in adjacency matrix
    adjMatrix.resize(MAX_NODES, std::bitset<MAX_NODES>());

    for (auto& lst: nextRelationships) {
        for (int n : lst.second) {
            adjMatrix[lst.first][n] = true; // Access matrix in row-major order
        }
    }

    finalized = true;
}

void dfs(const int node, std::unordered_map<int, std::vector<int>>& adjLstGraph, std::unordered_set<int>& visitedT) {
    if (visitedT.find(node) != visitedT.end()) return;
    visitedT.insert(node); // Store the node if not visited

    auto it = adjLstGraph.find(node);
    if (it != adjLstGraph.end()) { // Searches adjacency list for neighbouring nodes
        for (int next : it->second) { // Recursively runs through neighbours
            dfs(next, adjLstGraph, visitedT);
        }
    }
}

bool NextExtractor::hasNextT() const {
    return !nextRelationships.empty();
}

bool NextExtractor::containsNextTFrom(int stmtNo) const {
    return nextRelationships.count(stmtNo) && // short-circuit evaluation
        !nextRelationships.at(stmtNo).empty();
}

bool NextExtractor::containsNextTTo(int stmtNo) const {
    if (!finalized) {
        throw std::runtime_error("Tried to compute nextT relationships without finalizing nextExtractor");
    }

    return revNextRelationships.count(stmtNo) && // short-circuit evaluation
        !revNextRelationships.at(stmtNo).empty();
}

bool NextExtractor::containsNextT(int stmtNoFrom, int stmtNoTo) const {
    if (!finalized) {
        throw std::runtime_error("Tried to compute nextT relationships without finalizing nextExtractor");
    }

    std::unordered_set<int> visitedT;

    // Unable to use auto type deduction as this is recursive
    std::function<bool(int)> shortCircuitDFS = [&](int node) {
        if (visitedT.insert(node).second) {
            if (node == stmtNoTo) {
                return true;
            }

            auto it = nextRelationships.find(node);
            if (it != nextRelationships.end()) {
                for (int next : it->second) {
                    if (shortCircuitDFS(next)) {
                        return true;
                    }
                }
            }
        }

        return false;
    };

    // Run DFS to obtain all transitive closure to node stmtNum
    auto it = this->nextRelationships.find(stmtNoFrom);
    if (it != this->nextRelationships.end()) { // Searches adjacency list for neighbouring nodes
        for (const int next : it->second) { // Recursively runs through neighbours
            if (shortCircuitDFS(next)) return true;
        }
    }


    return false;

}

std::vector<int> NextExtractor::getStatementsNextTFrom() {
    auto key_selector = [](auto pair){return pair.first;};
    std::vector<int> keys(nextRelationships.size());

    transform(nextRelationships.begin(), nextRelationships.end(), keys.begin(), key_selector);

    return keys;
}

std::vector<int> NextExtractor::getStatementsNextTTo() {
    if (!finalized) {
        throw std::runtime_error("Tried to compute nextT relationships without finalizing nextExtractor");
    }

    auto key_selector = [](auto pair){return pair.first;};
    std::vector<int> keys(revNextRelationships.size());

    transform(revNextRelationships.begin(), revNextRelationships.end(), keys.begin(), key_selector);

    return keys;
}

std::vector<int> NextExtractor::getStatementsNextTFrom(int stmtNum) {
    if (!finalized) {
        throw std::runtime_error("Tried to compute nextT relationships without finalizing nextExtractor");
    }

    std::unordered_set<int> visitedT;

    // Run DFS to obtain all transitive closure from node stmtNum
    auto it = this->nextRelationships.find(stmtNum);
    if (it != this->nextRelationships.end()) { // Searches adjacency list for neighbouring nodes
        for (int next : it->second) { // Recursively runs through neighbours
            dfs(next, this->nextRelationships, visitedT);
        }
    }

    // Convert std::unordered_set to std::vector because QPS requires it as std::vector
    std::vector<int> visitedTArray;
    visitedTArray.insert(visitedTArray.end(), visitedT.begin(), visitedT.end());

    return visitedTArray;

}

std::vector<int> NextExtractor::getStatementsNextTTo(int stmtNum) {
    if (!finalized) {
        throw std::runtime_error("Tried to compute nextT relationships without finalizing nextExtractor");
    }

    std::unordered_set<int> visitedT;

    // Run DFS to obtain all transitive closure to node stmtNum
    auto it = this->revNextRelationships.find(stmtNum);
    if (it != this->revNextRelationships.end()) { // Searches adjacency list for neighbouring nodes
        for (int next : it->second) { // Recursively runs through neighbours
            dfs(next, this->revNextRelationships, visitedT);
        }
    }

    // Convert std::unordered_set to std::vector because QPS requires it as std::vector
    std::vector<int> visitedTArray;
    visitedTArray.insert(visitedTArray.end(), visitedT.begin(), visitedT.end());

    return visitedTArray;
}

std::vector<std::pair<int, int>> NextExtractor::getNextT() {
    if (!finalized) {
        throw std::runtime_error("Tried to compute NextT relationships before finalizing next relationships");
    }

    // Copy original graph of Next relationships into new graph so that
    // computed graph is dropped after function scope.
    // Uses adjacency matrix
    auto graph = std::make_unique<std::vector<std::bitset<MAX_NODES>>>(adjMatrix);

    const size_t numThreads = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;

    // Use modified Floyd-Warshall to compute transitive closure

    // Update step: Reachable(i,j,k) = Reachable(i, j, k - 1) OR
    // ( Reachable(i, k, k) AND Reachable(i, k, k) )
    auto updatePath = [&](std::vector<std::bitset<MAX_NODES>>& graph, int k, int i, int j) {
        graph[i][j] = graph[i][j] || (graph[i][k] && graph[k][j]);
    };

    for (int k = 0; k < MAX_NODES; ++k) {
        auto updateMatrix = [this, k, &graph, updatePath](int start, int end) {
            for (int i = start; i < end; ++i) {
                for (int j = 0; j < MAX_NODES; ++j) {
                    updatePath(*graph, k, i, j);
                }
            }
        };

        // Divide rows among tasks for parallel processing
        const size_t rowsPerTask = MAX_NODES / numThreads;
        for (int t = 0; t < numThreads; ++t) {
            size_t start = t * rowsPerTask;
            size_t end = (t + 1) * rowsPerTask;
            if (t == numThreads - 1) {
                end = MAX_NODES; // Handle any remaining rows in the last task
            }
            futures.emplace_back(std::async(std::launch::async, updateMatrix, start, end));
        }

        // Wait for all tasks to complete for this k
        for (auto& f : futures) {
            f.get(); // This blocks until the future is ready
        }
        futures.clear(); // Clear the futures for the next iteration
    }

    // Adjacency matrix stores all NextT relationships at this point.

    // Now, store them in an array.
    std::vector<std::pair<int, int>> nextTRelationships;

    for (size_t i = 0; i < graph->size(); ++i) {
        for (size_t j = 0; j < graph->at(i).size(); ++j) {
            if (graph->at(i)[j]) {
                nextTRelationships.emplace_back(i, j);
            }
        }
    }

    return nextTRelationships;
}
