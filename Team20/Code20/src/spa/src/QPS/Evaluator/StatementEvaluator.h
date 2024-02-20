#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class StatementEvaluator : private ClauseEvaluator {
    public:
        StatementEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, const ResultStore &resultStore)
            : ClauseEvaluator(r, cl, resultStore) {};

        bool evaluate() override;
    };
}