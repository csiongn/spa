#include <vector>
#include <utility>

#include "RuntimeExtractor.h"

bool RuntimeExtractor::hasNextT() {
  return false;
}

bool RuntimeExtractor::containsNextT(int stmtNoFrom, int stmtNoTo) {
  return false;
}

bool RuntimeExtractor::containsNextTFrom(int stmtNo) {
  return false;
}

bool RuntimeExtractor::containsNextTTo(int stmtNo) {
  return false;
}

std::vector<int> RuntimeExtractor::getStatementsNextTFrom(int stmtNo) {
  return std::vector<int>();
}

std::vector<int> RuntimeExtractor::getStatementsNextTTo(int stmtNo) {
  return std::vector<int>();
}

std::vector<int> RuntimeExtractor::getStatementsNextTFrom() {
  return std::vector<int>();
}

std::vector<int> RuntimeExtractor::getStatementsNextTTo() {
  return std::vector<int>();
}

std::vector<std::pair<int, int>> RuntimeExtractor::getNextT() {
  return std::vector<std::pair<int, int>>();
}

bool RuntimeExtractor::hasAffects() {
  return false;
}

bool RuntimeExtractor::containsAffects(int stmtNoFrom, int stmtNoTo) {
  return false;
}

bool RuntimeExtractor::containsAffectsFrom(int stmtNo) {
  return false;
}

bool RuntimeExtractor::containsAffectsTo(int stmtNo) {
  return false;
}

std::vector<int> RuntimeExtractor::getStatementsAffectsFrom(int stmtNo) {
  return std::vector<int>();
}

std::vector<int> RuntimeExtractor::getStatementsAffectsTo(int stmtNo) {
  return std::vector<int>();
}

std::vector<int> RuntimeExtractor::getStatementsAffectsFrom() {
  return std::vector<int>();
}

std::vector<int> RuntimeExtractor::getStatementsAffectsTo() {
  return std::vector<int>();
}

std::vector<std::pair<int, int>> RuntimeExtractor::getAffects() {
  return std::vector<std::pair<int, int>>();
}
