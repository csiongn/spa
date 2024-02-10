#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "PKB/facade/IPKBReader.h"
#include "Result.h"

namespace QueryEvaluator {
    class ClauseEvaluator {
    protected:
        std::shared_ptr<IPKBReader> reader;
        PQL::Clause clause;

    public:
        ClauseEvaluator(std::shared_ptr<IPKBReader> r, PQL::Clause cl)
            : reader(r), clause(cl) {};

        virtual Result evaluate() = 0;
    };
}
