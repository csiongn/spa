#include <string>
#include <vector>

#include "PQL.h"
#include "SimpleProgram.h"

namespace PQL {

    Synonym::Synonym(const SimpleProgram::DesignEntity &entity, const std::string &id)
        : entityType(entity), identity(id) {}

    Clause::Clause(const SimpleProgram::DesignAbstraction &abstraction, const std::vector<Synonym> &args)
        : clauseType(abstraction), arguments(args) {}

    Query::Query(const std::vector<Synonym> &dec, const std::vector<Clause> &cls, const PQL::Synonym &select)
        : declarations(dec), clauses(cls), selectSynonym(select){}
}