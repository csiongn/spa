#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class AssignPatternEvaluator : private ClauseEvaluator {
    public:
        AssignPatternEvaluator(std::shared_ptr<IPKBReader> r, PQL::Clause cl)
                : ClauseEvaluator(r, cl) {};

        Result evaluate() override;
    };
}