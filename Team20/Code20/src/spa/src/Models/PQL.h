#pragma once

#include <string>
#include <vector>

#include "SimpleProgram.h"

namespace PQL {
struct Synonym {
  SimpleProgram::DesignEntity entityType;
  std::string identity;
  SimpleProgram::AttributeRef attribute;

  static Synonym createInvalidSynonym();

  Synonym(const SimpleProgram::DesignEntity &entity, const std::string &id);

  Synonym(const SimpleProgram::DesignEntity &entity,
		  const std::string &id,
		  const SimpleProgram::AttributeRef &attribute);

  bool operator==(const Synonym &other) const {
	return entityType == other.entityType && identity == other.identity && attribute == other.attribute;
  };

  bool operator!=(const Synonym &other) const {
	return entityType != other.entityType || identity != other.identity || attribute != other.attribute;
  };
};

struct Clause {
  SimpleProgram::DesignAbstraction clauseType;
  std::vector<Synonym> arguments;
  bool isNegated = false;

  Clause(const SimpleProgram::DesignAbstraction &abstraction, const std::vector<Synonym> &args);

  void setNotClause();

  bool operator==(const Clause &other) const {
	return clauseType == other.clauseType && arguments == other.arguments && isNegated == other.isNegated;
  };

  bool operator!=(const Clause &other) const {
	return clauseType != other.clauseType || arguments != other.arguments || isNegated != other.isNegated;
  };
};

struct Query {
  std::vector<Synonym> declarations;
  std::vector<Clause> clauses;
  std::vector<Synonym> selectSynonyms;

  Query(const std::vector<Synonym> &dec, const std::vector<Clause> &cls, const std::vector<Synonym> &selects);

  bool operator==(const Query &other) const {
	return declarations == other.declarations && clauses == other.clauses && selectSynonyms == other.selectSynonyms;
  };

  bool operator!=(const Query &other) const {
	return declarations != other.declarations || clauses != other.clauses || selectSynonyms != other.selectSynonyms;;
  };
};
}