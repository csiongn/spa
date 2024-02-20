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
        ResultStore resultStore;

    public:
        ClauseEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, const ResultStore &resultStore)
            : reader(r), clause(cl), resultStore(resultStore) {};

        virtual bool evaluate() = 0;
    };
}
