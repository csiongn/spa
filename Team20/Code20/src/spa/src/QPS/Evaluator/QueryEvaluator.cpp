#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "PKB/facade/IPKBReader.h"
#include "QueryEvaluator.h"
#include "Result.h"

namespace QueryEvaluator {
    QueryEvaluator::QueryEvaluator(std::shared_ptr<IPKBReader> r) : results(), reader(r) {}

    std::vector<std::string> evaluateQuery(const PQL::Query &q) {
        // TODO

        return {"test"};
    }

    Result evaluateClause(PQL::Clause clause) {
        // TODO

        return Result("test", {"test"});
    }
}
