#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class EntityEvaluator : private ClauseEvaluator {
    public:
        EntityEvaluator(std::shared_ptr<IPKBReader> r, PQL::Clause cl)
                : ClauseEvaluator(r, cl) {};

        Result evaluate() override;
    };
}