#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class AssignPatternEvaluator : private ClauseEvaluator {
    public:
        AssignPatternEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl,
                               std::shared_ptr<ResultStore> resultStore1)
                : ClauseEvaluator(r, cl, resultStore1) {};

        bool evaluate() override;
    };
}