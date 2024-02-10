#include "Result.h"

namespace QueryEvaluator {
    Result::Result(const std::string &colName, const std::vector<std::string> &values) {
        colNames.insert(colName);
        table[colName] = values;
    }

    Result::Result(const std::unordered_map<std::string, std::vector<std::string>> &t,
                   std::unordered_set<std::string> &cols)
                   : table(t), colNames(cols) {}
}