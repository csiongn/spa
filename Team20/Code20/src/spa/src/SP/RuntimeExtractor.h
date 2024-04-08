#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "control_flow_graph/CFG.h"
#include "control_flow_graph/NextExtractor.h"

class RuntimeExtractor {

	// Attributes
	std::shared_ptr<CFGManager> cfgManager;
	std::shared_ptr<NextExtractor> nextExtractor;

 public:
	// Constructor
	RuntimeExtractor(std::shared_ptr<CFGManager> cfgManager, std::shared_ptr<NextExtractor> nextExtractor)
	: cfgManager(std::move(cfgManager)), nextExtractor(std::move(nextExtractor)) {}

	// Checks whether any NextT relationship exists
	bool hasNextT();

	// Checks whether the relationship NextT(stmtNoFrom, stmtNoTo) exists
	bool containsNextT(int stmtNoFrom, int stmtNoTo);;

	// Checks whether there is any NextT relationship such that NextT(stmtNo, _)
	bool containsNextTFrom(int stmtNo);

	// Checks whether there is any NextT relationship such that NextT(_, stmtNo)
	bool containsNextTTo(int stmtNo);
	// Returns all statement with number i such that NextT(stmtNo, i)
	std::vector<int> getStatementsNextTFrom(int stmtNo);;

	// Returns all statement with number i such that NextT(i, stmtNo);
	std::vector<int> getStatementsNextTTo(int stmtNo);;

	// Returns all statements with number i where there exists NextT(i, _);
	std::vector<int> getStatementsNextTFrom();

	// Returns all statements with number i where there exists NextT(_, i);
	std::vector<int> getStatementsNextTTo();

	// Returns all NextT relationships
	std::vector<std::pair<int, int> > getNextT();

	// Checks whether any Affects relatoinship exists
	bool hasAffects();

	// Checks whether the relationship Affects(stmtNoFrom, stmtNoTo) exists
	bool containsAffects(int stmtNoFrom, int stmtNoTo);

	// Checks whether there is any Affects relationship such that Affects(stmtNo, _)
	bool containsAffectsFrom(int stmtNo);

	// Checks whether there is any Affects relationship such that Affects(_, stmtNo)
	bool containsAffectsTo(int stmtNo);

	// Returns all statement with number i such that Affects(stmtNo, i)
	std::vector<int> getStatementsAffectsFrom(int stmtNo);

	// Returns all statement with number i such that Affects(i, stmtNo);
	std::vector<int> getStatementsAffectsTo(int stmtNo);

	// Returns all statements with number i where there exists Affects(i, _);
	std::vector<int> getStatementsAffectsFrom();

	// Returns all statements with number i where there exists Affects(_, i);
	std::vector<int> getStatementsAffectsTo();

	// Returns all NextT relationships
	std::vector<std::pair<int, int> > getAffects();
};
