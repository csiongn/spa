#pragma once

#include <memory>
#include <string>
#include <vector>

#include "RelationshipEvaluator.h"

namespace QueryEvaluator {
class EntityEvaluator : private RelationshipEvaluator {
 public:
  EntityEvaluator(std::shared_ptr<IPKBReader> r,
				  const PQL::Clause &cl,
				  std::shared_ptr<ResultStore> resultStore,
				  bool createTable)
	  : RelationshipEvaluator(r, cl, resultStore, createTable) {};

  bool evaluate() override;

 private:
  bool isAlwaysFalse() override;

  bool hasRelationship() override;

  bool
  hasRelationship(const SimpleProgram::DesignAbstraction &relationship, int stmtNum, const std::string &ident);

  bool
  hasRelationship(const SimpleProgram::DesignAbstraction &relationship, const std::string &lIdent,
				  const std::string &rIdent);

  bool hasAtLeastOneRelationship() override;

  bool getForwardRelationship() override;

  bool getReversedRelationship() override;

  bool getLeftResults() override;

  bool getSynonymWildcard() override;

  bool getRightResults() override;

  bool getWildcardSynonym() override;

  bool getDoubleSynonym() override;

  std::vector<int> getUniqueStmtNums(const PQL::Synonym &syn);

  std::vector<std::string> getUniqueLeftProcNames();

  std::vector<std::string> getUniqueLeftProcNames(const std::string &ident);

  std::vector<std::string> getUniqueValues();

  std::vector<std::string> getUniqueValues(const PQL::Synonym lArg);

  std::vector<std::string> negateStringResults(const PQL::Synonym &syn, const std::vector<std::string> &selected);

  std::vector<std::string> getUniqueIdent(const PQL::Synonym &syn);
};
}