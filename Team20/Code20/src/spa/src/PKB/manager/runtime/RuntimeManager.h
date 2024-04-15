# pragma once

# include <unordered_map>
# include <string>
# include <memory>
# include <unordered_set>
# include <vector>
#include <utility>

#include "PKB/utils/MapUtils.h"
#include "SP/RuntimeExtractor.h"

// T will be the type of the runtime extractor
class RuntimeManager {
  // will store runtime extractor
  // allow insert runtime extractor
  // different methods to interact with runtime extractor, dont violate LoD, by exposing runtime extractor api
 private:
  std::shared_ptr<RuntimeExtractor> runtimeExtractor;

 public:
  // insert runtime extractor
  void insert(std::shared_ptr<RuntimeExtractor> extractor) {
	this->runtimeExtractor = extractor;
  };

  // NextT
  bool containsNextT(int stmtNum) {
	return runtimeExtractor->containsNextTTo(stmtNum);
  };

  bool containsNextTReverse(int stmtNum) {
	return runtimeExtractor->containsNextTFrom(stmtNum);
  };

  bool containsNextTRelationship(int stmtNum, int nextStmtNum) {
	return runtimeExtractor->containsNextT(stmtNum, nextStmtNum);
  };

  bool hasNextTRelationship() {
	return runtimeExtractor->hasNextT();
  };

  std::vector<int> getNextT(int stmtNum) {
	return runtimeExtractor->getStatementsNextTTo(stmtNum);
  };

  std::vector<int> getNextTReverse(int stmtNum) {
	return runtimeExtractor->getStatementsNextTFrom(stmtNum);
  };

  std::vector<int> getNextT() {
	return runtimeExtractor->getStatementsNextTTo();
  };
  std::vector<int> getNextTReverse() {
	return runtimeExtractor->getStatementsNextTFrom();
  };

  std::vector<std::pair<int, int>> getNextTRelationships() {
	return runtimeExtractor->getNextT();
  };

  // Affects
  bool containsAffects(int stmtNum) {
	return runtimeExtractor->containsAffectsTo(stmtNum);
  };

  bool containsAffectsReverse(int stmtNum) {
	return runtimeExtractor->containsAffectsFrom(stmtNum);
  };

  bool containsAffectsRelationship(int stmtNum, int nextStmtNum) {
	return runtimeExtractor->containsAffects(stmtNum, nextStmtNum);
  };

  bool hasAffectsRelationship() {
	return runtimeExtractor->hasAffects();
  };

  std::vector<int> getAffects(int stmtNum) {
	return runtimeExtractor->getStatementsAffectsTo(stmtNum);
  };

  std::vector<int> getAffectsReverse(int stmtNum) {
	return runtimeExtractor->getStatementsAffectsFrom(stmtNum);
  };

  std::vector<int> getAffects() {
	return runtimeExtractor->getStatementsAffectsTo();
  };
  std::vector<int> getAffectsReverse() {
	return runtimeExtractor->getStatementsAffectsFrom();
  };

  std::vector<std::pair<int, int>> getAffectsRelationships() {
	return runtimeExtractor->getAffects();
  };
};
