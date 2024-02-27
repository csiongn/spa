#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "PKB/facade/IPKBReader.h"
#include "Result.h"
#include "ResultStore.h"

namespace QueryEvaluator {
    class ClauseEvaluator {
    protected:
        std::shared_ptr<IPKBReader> reader;
        PQL::Clause clause;
        std::shared_ptr<ResultStore> resultStore;

    public:
        ClauseEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, std::shared_ptr<ResultStore> resultStore1)
                : reader(r), clause(cl) {
            resultStore = resultStore1;
        };

        virtual bool evaluate() = 0;

        static std::vector<int> getIntersection(std::vector<int> &v1, std::vector<int> &v2);

        virtual bool isAlwaysFalse() = 0;

        virtual bool hasRelationship() = 0;

        virtual bool hasAtLeastOneRelationship() = 0;

        virtual bool getForwardRelationship() = 0;

        virtual bool getReversedRelationship() = 0;

        virtual bool getLeftResults() = 0;

        virtual bool getSynonymWildcard() = 0;

        virtual bool getRightResults() = 0;

        virtual bool getWildcardSynonym() = 0;

        virtual bool getDoubleSynonym() = 0;

        std::vector<int> getStmtNums(const PQL::Synonym &syn);
    };
}
