#include <unordered_set>

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

        joinResults();
        if (results.empty()) {
            return {};
        }

        Result res = *(results[0].get());
        size_t selectSynIndex = res.colNameToIndex.at(selectSyn.identity);
        std::vector<std::string> selectResult = res.table[selectSynIndex];
        return removeDuplicates(selectResult);
    }

    void ResultStore::joinResults() {
        Result *t1;
        Result *t2;
        std::vector<std::string> commonCols;
        std::shared_ptr<Result> newResult;
        while (results.size() > 1) {
            t1 = results[0].get();
            t2 = results[1].get();
            commonCols = t1->getCommonColumns(*t2);
            if (commonCols.empty()) {
                // cross join
                newResult = t1->crossJoin(*t2);
            } else {
                // hash join
                newResult = t1->hashJoin(*t2, commonCols);
                if (newResult->empty) {
                    results.clear();
                    return;
                }
            }

            results.erase(results.begin());
            results.erase(results.begin());
            results.push_back(newResult);
        }
    }

    std::vector<std::string> ResultStore::removeDuplicates(std::vector<std::string> column) {
        std::unordered_set<std::string> added;
        std::vector<std::string> noDuplicates;
        for (auto &value: column) {
            if (added.count(value) > 0) {
                continue;
            }
            added.insert(value);
            noDuplicates.push_back(value);
        }

        return noDuplicates;
    }
}
