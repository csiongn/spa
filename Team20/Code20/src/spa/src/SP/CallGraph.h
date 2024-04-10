#pragma once

#include <bitset>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

#include "PKB/facade/IPKBWriter.h"

constexpr size_t MAX_NODES = 600;
class CallGraph {

  // Attributes
  std::unordered_map<std::string, int> nameToIndex; // Mapping from procedure name to index
  std::vector<std::string> indexToName; // Reverse mapping from index (stored as vector index) to procedure name
  std::vector<std::unordered_set<int>> adjLst;
  std::vector<std::bitset<MAX_NODES>> adjMatrix;
  int index = 0;
  bool finalized = false;

  // Methods
  void computeTransitiveCall();

 public:
  void addCall(const std::string& caller, const std::string& called);
  void finalize();
  void pushToPKB(const std::shared_ptr<IPKBWriter>& pkbWriter);
  void pushToPKB(const std::shared_ptr<IPKBWriter>& pkbWriter, std::unordered_map<std::string, std::unordered_set<std::string>> procsUses,
    std::unordered_map<std::string, std::unordered_set<std::string>> procsModifies);
  bool hasCallsRelationship(const std::string& a, const std::string& b);
  bool hasCallsTRelationship(const std::string& a, const std::string& b);
};