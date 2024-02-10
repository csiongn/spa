#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "PKB/facade/IPKBReader.h"
#include "Result.h"

namespace QueryEvaluator {
    class QueryEvaluator {
    public:
        std::vector<Result> results;
        std::shared_ptr<IPKBReader> reader;

        QueryEvaluator(std::shared_ptr<IPKBReader> r);

        std::vector<std::string> evaluateQuery(const PQL::Query &q);

    private:
        Result evaluateClause(PQL::Clause clause);
    };
}
