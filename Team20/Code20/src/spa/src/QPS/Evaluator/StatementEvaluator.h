#pragma once

#include <memory>
#include <vector>

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
class StatementEvaluator : private ClauseEvaluator {
 public:
  StatementEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl,
					 std::shared_ptr<ResultStore> resultStore1)
	  : ClauseEvaluator(r, cl, resultStore1) {
  };

  bool evaluate() override;

 private:
  bool isAlwaysFalse() override;

  bool hasRelationship() override;

  bool hasRelationship(const SimpleProgram::DesignAbstraction &relationship, int leftStmtNum, int rightStmtNum);

  bool hasAtLeastOneRelationship() override;

  bool getForwardRelationship() override;

  bool getReversedRelationship() override;

  bool getLeftResults() override;

  bool getSynonymWildcard() override;

  bool getRightResults() override;

  bool getWildcardSynonym() override;

  bool getDoubleSynonym() override;

  std::vector<int> getUniqueKeys(const PQL::Synonym &syn);

  std::vector<int> getUniqueValues(const PQL::Synonym &syn);
};
}