#pragma once

#include <vector>
#include <utility>

class RuntimeExtractor {

 public:
  // Checks whether any NextT relationship exists
  bool hasNextT() {
	return false;
  };
    // Checks whether the relationship NextT(stmtNoFrom, stmtNoTo) exists
	bool containsNextT(int stmtNoFrom, int stmtNoTo) {
	  // TODO: To be implemented
	  return false;
	};
    // Checks whether there is any NextT relationship such that NextT(stmtNo, _)
	bool containsNextTFrom(int stmtNo) {
	  // TODO: To be implemented
	  return false;
	};
    // Checks whether there is any NextT relationship such that NextT(_, stmtNo)
	bool containsNextTTo(int stmtNo) {
	  // TODO: To be implemented
	  return false;
	};
    // Returns all statement with number i such that NextT(stmtNo, i)
	std::vector<int> getStatementsNextTFrom(int stmtNo) {
	  // TODO: To be implemented
	  return std::vector<int>();
	};
    // Returns all statement with number i such that NextT(i, stmtNo);
	std::vector<int> getStatementsNextTTo(int stmtNo) {
	  // TODO: To be implemented
	  return std::vector<int>();
	};
    // Returns all statements with number i where there exists NextT(i, _);
	std::vector<int> getStatementsNextTFrom() {
	  // TODO: To be implemented
	  return std::vector<int>();
	};
    // Returns all statements with number i where there exists NextT(_, i);
	std::vector<int> getStatementsNextTTo() {
	  // TODO: To be implemented
	  return std::vector<int>();
	};
    // Returns all NextT relationships
	std::vector<std::pair<int, int> > getNextT() {
	  // TODO: To be implemented
	  return std::vector<std::pair<int, int>>();
	};

    // Checks whether any Affects relatoinship exists
	bool hasAffects() {
	  // TODO: To be implemented
	  return false;
	};
    // Checks whether the relationship Affects(stmtNoFrom, stmtNoTo) exists
	bool containsAffects(int stmtNoFrom, int stmtNoTo) {
	  // TODO: To be implemented
	  return false;
	};
    // Checks whether there is any Affects relationship such that Affects(stmtNo, _)
	bool containsAffectsFrom(int stmtNo) {
	  // TODO: To be implemented
	  return false;
	};
    // Checks whether there is any Affects relationship such that Affects(_, stmtNo)
	bool containsAffectsTo(int stmtNo) {
	  // TODO: To be implemented
	  return false;
	};
    // Returns all statement with number i such that Affects(stmtNo, i)
	std::vector<int> getStatementsAffectsFrom(int stmtNo) {
	  // TODO: To be implemented
	  return std::vector<int>();
	};
    // Returns all statement with number i such that Affects(i, stmtNo);
	std::vector<int> getStatementsAffectsTo(int stmtNo) {
	  // TODO: To be implemented
	  return std::vector<int>();
	};
    // Returns all statements with number i where there exists Affects(i, _);
	std::vector<int> getStatementsAffectsFrom() {
	  // TODO: To be implemented
	  return std::vector<int>();
	};
    // Returns all statements with number i where there exists Affects(_, i);
	std::vector<int> getStatementsAffectsTo() {
	  // TODO: To be implemented
	  return std::vector<int>();
	};
    // Returns all NextT relationships
	std::vector<std::pair<int, int> > getAffects() {
	  // TODO: To be implemented
	  return std::vector<std::pair<int, int>>();
	};
};
