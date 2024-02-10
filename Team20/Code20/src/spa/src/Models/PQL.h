#pragma once

#include <string>
#include <vector>

#include "SimpleProgram.h"

namespace PQL {
    struct Synonym {
        SimpleProgram::DesignEntity entityType;
        std::string identity;

        Synonym(const SimpleProgram::DesignEntity &entity, const std::string& id);
    };

    struct Clause {
        SimpleProgram::DesignAbstraction clauseType;
        std::vector<Synonym> arguments;

        Clause(const SimpleProgram::DesignAbstraction &abstraction, const std::vector<Synonym> &args);
    };

    struct Query {
        std::vector<Synonym> declarations;
        std::vector<Clause> clauses;
        Synonym selectSynonym;

        Query(const std::vector<Synonym> &dec, const std::vector<Clause> &cls, const Synonym &select);
    };
}