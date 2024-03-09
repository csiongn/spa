# pragma once

# include <unordered_map>
# include <string>
# include <memory>
# include <unordered_set>
# include <vector>
#include "SP/AST.h"

template <typename T, typename U>
class ConditionalPatternManager {
private:
    // T is variable string, U is stmt_no
    std::unordered_map<T, std::unordered_set<U> > data;
    std::unordered_map<U, std::unordered_set<T> > reverseData;

public:

    // Insert a record into the table
    // T is variable string, U is stmtNo
    void insert(const T& key, const U& value) {
        data[key].insert(value);
        reverseData[value].insert(key);
    };

    // Get the value associated with a key
    std::vector<U> get(const T &key) {
        if (data.find(key) == data.end()) {
            return std::vector<U>();
        }
        // convert set at data to vector
        return std::vector(data.at(key).begin(), data.at(key).end());
    };

    // Get the key associated with a value
    std::vector<T> getReverse(const U& value) {
        // No such key exists for reverse data
        if (reverseData.find(value) == reverseData.end()) {
            return std::vector<T>();
        }
        return std::vector(reverseData.at(value).begin(), reverseData.at(value).end());
    };

    std::vector<T> getKeys() {
        std::vector<T> keys;
        for (const auto&[key, value] : data) {
            keys.push_back(key);
        }
        return keys;
    }

    std::vector<U> getValues() {
        std::vector<U> values;
        for (const auto&[key, value] : reverseData) {
            values.push_back(key);
        }
        return values;
    }

    // Check if a key exists in the table
    bool contains(const T& key) {
        return data.find(key) != data.end();
    };

    // Check if a value exists in the table
    bool containsReverse(const U& value) {
        return reverseData.find(value) != reverseData.end();
    };

    bool containsPattern(const T& key, const U& value) {
        if (contains(key)) {
            const auto &valueSet= data[key];
            return (valueSet.find(value) != valueSet.end());
        }
        return false; // key does not exist
    }

    bool hasPattern() {
        return !data.empty();
    }
};
