#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class StatementEvaluator : private ClauseEvaluator {
    public:
        StatementEvaluator(std::shared_ptr<IPKBReader> r, PQL::Clause cl)
            : ClauseEvaluator(r, cl) {};

        Result evaluate() override;
    };
}