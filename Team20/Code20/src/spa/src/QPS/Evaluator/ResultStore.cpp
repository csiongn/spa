#include "ResultStore.h"

namespace QueryEvaluator {
    void ResultStore::insertResult(std::shared_ptr<Result> res) {
        results.push_back(res);
    }

    void ResultStore::createColumn(const PQL::Synonym &syn, const std::vector<std::string> &values) {
        std::shared_ptr<Result> res = std::make_shared<Result>(syn.identity, values);
        results.push_back(res);
    }

    void ResultStore::createColumn(const PQL::Synonym &syn, const std::vector<int> &values) {
        std::shared_ptr<Result> res = std::make_shared<Result>(syn.identity, values);
        results.push_back(res);
    }

    std::vector<std::string> ResultStore::retrieveSelect(const PQL::Synonym &selectSyn) {
        if (results.empty()) {
            // no result
            return {};
        }

        Result* res = results[0].get();
        return res->table[selectSyn.identity];
    }
}
