#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class EntityEvaluator : private ClauseEvaluator {
    public:
        EntityEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl, ResultStore &resultStore)
                : ClauseEvaluator(r, cl, resultStore) {};

        bool evaluate() override;
    };
}