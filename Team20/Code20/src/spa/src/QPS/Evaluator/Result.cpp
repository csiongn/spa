#include <algorithm>

#include "Result.h"

namespace QueryEvaluator {
    Result::Result(const std::string &colName, const std::vector<std::string> &values) {
        colNames.insert(colName);
        table[colName] = values;
    }

    Result::Result(const std::string &colName, const std::vector<int> &values) {
        colNames.insert(colName);
        std::vector<std::string> stringVector(values.size());
        std::transform(values.begin(), values.end(), stringVector.begin(), [](int intVal){
            return std::to_string(intVal);
        });
        table[colName] = stringVector;
    }

    Result::Result(const std::unordered_map<std::string, std::vector<std::string>> &t,
                   std::unordered_set<std::string> &cols)
                   : table(t), colNames(cols) {}

    bool Result::isEmpty() const {
        return table.empty();
    }
}