# pragma once

# include <unordered_map>
# include <string>
# include <memory>
# include <unordered_set>
# include <vector>
#include "SP/AST.h"

template <typename T, typename U, typename V>
class PatternManager {
private:
    // T is LHS, U is RHS
    // U for Nodes need to be size_t cause it is hash value

    // LHS: RHS Hash
    std::unordered_map<T, std::unordered_set<U> > data;

    // RHS Hash
    std::unordered_map<U, std::unordered_set<T> > reverseData;

    // stmtNum of RHS and LHS
    std::unordered_map<T, std::unordered_set<int>> stmtData;
    std::unordered_map<U, std::unordered_set<int>> stmtReverseData;
    // hash to pointer to node
    std::unordered_map<U, std::unordered_set<V> > nodePtrMap;


public:

    // Insert a record into the table
    // T is variable string, U is hash value of node, stmtNo, Ptr to Node
    void insert(const T& key, const U& value, const int& stmtNum, const V& node) {
        data[key].insert(value);
        reverseData[value].insert(key);
        stmtData[key].insert(stmtNum);
        stmtReverseData[value].insert(stmtNum);
        nodePtrMap[value].insert(node);
    };

    // Get the value associated with a key
    std::vector<U> getRHS(const T &key) {
        if (data.find(key) == data.end()) {
            return std::vector<U>();
        }
        // convert set at data to vector
        return std::vector(data.at(key).begin(), data.at(key).end());
    };

    // Get the key associated with a value
    std::vector<T> getLHS(const U& value) {
        // No such key exists for reverse data
        if (reverseData.find(value) == reverseData.end()) {
            return std::vector<T>();
        }
        return std::vector(reverseData.at(value).begin(), reverseData.at(value).end());
    };

    // getAllLHS
    std::vector<T> getLHSKeys() {
        std::vector<T> keys;
        for (const auto&[key, value] : data) {
            keys.push_back(key);
        }
        return keys;
    }

    // getAllRHS, return the hash
    std::vector<U> getRHSKeys() {
        std::vector<U> values;
        for (const auto&[key, value] : reverseData) {
            values.push_back(key);
        }
        return values;
    }

    std::vector<int> getPatternStmtNum(const T& key, const U& value) {
        std::vector<int> stmtNums;
        if (stmtData.find(key) != stmtData.end()) {
            for (const auto& stmtNum : stmtData[key]) {
                if (stmtReverseData[value].find(stmtNum) != stmtReverseData[value].end()) {
                    stmtNums.push_back(stmtNum);
                }
            }
        }
        return stmtNums;
    }

    std::vector<int> getLHSStmtNum(const T& key) {
        if (stmtData.find(key) == stmtData.end()) {
            return std::vector<int>();
        }
        return std::vector(stmtData.at(key).begin(), stmtData.at(key).end());
    }

    std::vector<int> getRHSStmtNum(const U& value) {
        if (stmtReverseData.find(value) == stmtReverseData.end()) {
            return std::vector<int>();
        }
        return std::vector(stmtReverseData.at(value).begin(), stmtReverseData.at(value).end());
    }

    // get V
    std::vector<V> getRHSNodePtr(const U& value) {
        if (nodePtrMap.find(value) == nodePtrMap.end()) {
            return std::vector<V>();
        }
        return std::vector(nodePtrMap.at(value).begin(), nodePtrMap.at(value).end());
    }

    std::vector<V> getRHSNodePtr() {
        std::vector<V> nodePtrs;
        for (const auto&[key, value] : nodePtrMap) {
            for (const auto& nodePtr : value) {
                nodePtrs.push_back(nodePtr);
            }
        }
        return nodePtrs;
    }

    // Check if a key exists in the table
    bool containsLHS(const T& key) {
        return data.find(key) != data.end();
    };

    // Check if a value exists in the table
    bool containsRHS(const U& value) {
        return reverseData.find(value) != reverseData.end();
    };

    bool containsPattern(const T& key, const U& value) {
        if (containsLHS(key)) {
            const auto &valueSet= data[key];
            return (valueSet.find(value) != valueSet.end());
        }
        return false; // key does not exist
    }

    bool hasPattern() {
        return !data.empty();
    }
};