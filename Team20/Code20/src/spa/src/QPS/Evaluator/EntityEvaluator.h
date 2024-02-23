#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class EntityEvaluator : private ClauseEvaluator {
    public:
        EntityEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, std::shared_ptr<ResultStore> resultStore1)
                : ClauseEvaluator(r, cl, resultStore1) {};

        bool evaluate() override;

    private:
        bool isAlwaysFalse() override;

        bool hasRelationship() override;

        bool
        hasRelationship(const SimpleProgram::DesignAbstraction &relationship, int stmtNum, const std::string &ident);

        bool hasAtLeastOneRelationship() override;

        bool getForwardRelationship() override;

        bool getReversedRelationship() override;

        bool getLeftResults() override;

        bool getSynonymWildcard() override;

        bool getRightResults() override;

        bool getWildcardSynonym() override;

        bool getDoubleSynonym() override;

        std::vector<int> getUniqueKeys(const PQL::Synonym &syn);

        std::vector<std::string> getUniqueValues();
    };
}