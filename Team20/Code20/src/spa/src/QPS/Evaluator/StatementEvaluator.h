#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class StatementEvaluator : private ClauseEvaluator {
    public:
        StatementEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, const ResultStore &resultStore)
            : ClauseEvaluator(r, cl, resultStore) {};

        bool evaluate() override;

    private:
        static std::vector<int> getIntersection(std::vector<int> &v1, std::vector<int> &v2);

        bool isAlwaysFalse() override;
        bool hasRelationship() override;
        bool hasRelationship(const SimpleProgram::DesignAbstraction &relationship, int leftStmtNum, int rightStmtNum) override;
        bool hasAtLeastOneRelationship() override;
        bool getForwardRelationship() override;
        bool getReversedRelationship() override;
        bool getLeftResults() override;
        bool getSynonymWildcard() override;
        bool getRightResults() override;
        bool getWildcardSynonym() override;
        bool getDoubleSynonym() override;

        std::vector<int> getStmtNums(const PQL::Synonym &syn);
        std::vector<int> getUniqueKeys(const PQL::Synonym &syn);
        std::vector<int> getUniqueValues(const PQL::Synonym &syn);
    };
}