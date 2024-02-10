#pragma once

#
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Models/PQL.h"

namespace QueryEvaluator {
    class Result {
    public:
        // key: column name, values: all the rows for that column
        std::unordered_map<std::string, std::vector<std::string>> table;
        std::unordered_set<std::string> colNames;

        // add by columns
        Result(const std::string &colName, const std::vector<std::string> &values);
        Result(const std::unordered_map<std::string, std::vector<std::string>> &t,
               std::unordered_set<std::string> &cols);
    };
}