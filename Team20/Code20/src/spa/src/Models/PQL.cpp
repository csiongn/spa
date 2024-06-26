#include <string>
#include <vector>

#include "PQL.h"
#include "SimpleProgram.h"

namespace PQL {

Synonym Synonym::createInvalidSynonym() {
  return {SimpleProgram::DesignEntity::INVALID, "", SimpleProgram::AttributeRef::NO_REF};
}

Synonym::Synonym(const SimpleProgram::DesignEntity &entity, const std::string &id)
	: entityType(entity), identity(id), attribute(SimpleProgram::AttributeRef::NO_REF) {}

Synonym::Synonym(const SimpleProgram::DesignEntity &entity,
				 const std::string &id,
				 const SimpleProgram::AttributeRef &attribute)
	: entityType(entity), identity(id), attribute(attribute) {}

Clause Clause::createEmptyClause() {
  return {SimpleProgram::DesignAbstraction::INVALID, {}};
}

Clause::Clause(const SimpleProgram::DesignAbstraction &abstraction, const std::vector<Synonym> &args)
	: clauseType(abstraction), arguments(args) {}

void Clause::setNotClause() {
  isNegated = true;
}

Query::Query(const std::vector<Synonym> &dec, const std::vector<Clause> &cls, const std::vector<PQL::Synonym> &selects)
	: declarations(dec), clauses(cls), selectSynonyms(selects) {}
}