#pragma once

#include <memory>
#include "Result.h"

namespace QueryEvaluator {
    class ResultStore {
    public:
        std::vector<std::shared_ptr<Result>> results;

        ResultStore() = default;

        void insertResult(std::shared_ptr<Result> r);

        void createColumn(const PQL::Synonym &syn, const std::vector<std::string> &values);

        void createColumn(const PQL::Synonym &syn, const std::vector<int> &values);

        std::vector<std::string> retrieveSelect(const PQL::Synonym &selectSyn);

    private:
        void joinResults();
        std::vector<std::string> removeDuplicates(std::vector<std::string> v);
    };
}