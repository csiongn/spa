#pragma once

#include <string>
#include <vector>

#include "SimpleProgram.h"

namespace PQL {
    struct Synonym {
        SimpleProgram::DesignEntity entityType;
        std::string identity;

        Synonym(const SimpleProgram::DesignEntity &entity, const std::string& id);

        bool operator==(const Synonym& other) const {
            return entityType == other.entityType && identity == other.identity;
        };

        bool operator!=(const Synonym& other) const {
            return entityType != other.entityType || identity != other.identity;
        };
    };

    struct Clause {
        SimpleProgram::DesignAbstraction clauseType;
        std::vector<Synonym> arguments;

        Clause(const SimpleProgram::DesignAbstraction &abstraction, const std::vector<Synonym> &args);

        bool operator==(const Clause& other) const {
            return clauseType == other.clauseType && arguments == other.arguments;
        };

        bool operator!=(const Clause& other) const {
            return clauseType != other.clauseType || arguments != other.arguments;
        };
    };

    struct Query {
        std::vector<Synonym> declarations;
        std::vector<Clause> clauses;
        Synonym selectSynonym;

        Query(const std::vector<Synonym> &dec, const std::vector<Clause> &cls, const Synonym &select);

        bool operator==(const Query& other) const {
            return declarations == other.declarations && clauses == other.clauses && selectSynonym == other.selectSynonym;
        };

        bool operator!=(const Query& other) const {
            return declarations != other.declarations || clauses != other.clauses || selectSynonym != other.selectSynonym;;
        };
    };
}