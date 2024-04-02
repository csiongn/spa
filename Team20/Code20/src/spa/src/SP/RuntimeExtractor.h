#pragma once

#include <pair>
#include <unordered_set>
#include <utility>

class RuntimeExtractor {

    // Checks whether any NextT relatoinship exists
    bool isNextT();
    // Checks whether the relationship NextT(stmtNoFrom, stmtNoTo) exists
    bool isNextT(int stmtNoFrom, int stmtNoTo);
    // Checks whether there is any NextT relationship such that NextT(stmtNo, _)
    bool isNextTFrom(int stmtNo);
    // Checks whether there is any NextT relationship such that NextT(_, stmtNo)
    bool isNextTTo(int stmtNo);
    // Returns all statement with number i such that NextT(stmtNo, i)
    std::unordered_set<int> getStatementsNextTFrom(int stmtNo);
    // Returns all statement with number i such that NextT(i, stmtNo);
    std::unordered_set<int> getStatementsNextTTo(int stmtNo);
    // Returns all statements with number i where there exists NextT(i, _);
    std::unordered_set<int> getStatementsNextTFrom();
    // Returns all statements with number i where there exists NextT(_, i);
    std::unordered_set<int> getStatementsNextTTo();
    // Returns all NextT relationships
    std::unordered_set<std::pair<int, int> > getNextT();

    // Checks whether any Affects relatoinship exists
    bool isAffects();
    // Checks whether the relationship Affects(stmtNoFrom, stmtNoTo) exists
    bool isAffects(int stmtNoFrom, int stmtNoTo);
    // Checks whether there is any Affects relationship such that Affects(stmtNo, _)
    bool isAffectsFrom(int stmtNo);
    // Checks whether there is any Affects relationship such that Affects(_, stmtNo)
    bool isAffectsTo(int stmtNo);
    // Returns all statement with number i such that Affects(stmtNo, i)
    std::unordered_set<int> getStatementsAffectsFrom(int stmtNo);
    // Returns all statement with number i such that Affects(i, stmtNo);
    std::unordered_set<int> getStatementsAffectsTo(int stmtNo);
    // Returns all statements with number i where there exists Affects(i, _);
    std::unordered_set<int> getStatementsAffectsFrom();
    // Returns all statements with number i where there exists Affects(_, i);
    std::unordered_set<int> getStatementsAffectsTo();
    // Returns all NextT relationships
    std::unordered_set<std::pair<int, int> > getAffects();
}
