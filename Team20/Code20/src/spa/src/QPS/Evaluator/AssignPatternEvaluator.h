#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class AssignPatternEvaluator : private ClauseEvaluator {
    public:
        AssignPatternEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, const ResultStore &resultStore)
                : ClauseEvaluator(r, cl, resultStore) {};

        bool evaluate() override;
    };
}