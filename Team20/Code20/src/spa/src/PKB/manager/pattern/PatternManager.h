# pragma once

# include <unordered_map>
# include <string>
# include <memory>
# include <unordered_set>
# include <vector>
#include "SP/AST.h"
#include "PKB/utils/MapUtils.h"

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
        return MapUtils<T, U>::getValuesForKey(data, key);
    };

    // Get the key associated with a value
    std::vector<T> getLHS(const U& value) {
        return MapUtils<U, T>::getValuesForKey(reverseData, value);
    };

    // getAllLHS
    std::vector<T> getLHSKeys() {
        return MapUtils<T, U>::getAllKeys(data);
    }

    // getAllRHS, return the hash
    std::vector<U> getRHSKeys() {
        return MapUtils<U, T>::getAllKeys(reverseData);
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
        return MapUtils<T, int>::getValuesForKey(stmtData, key);
    }

    std::vector<int> getRHSStmtNum(const U& value) {
        return MapUtils<U, int>::getValuesForKey(stmtReverseData, value);
    }

    // get V
    std::vector<V> getRHSNodePtr(const U& value) {
        return MapUtils<U, V>::getValuesForKey(nodePtrMap, value);
    }

    std::vector<V> getRHSNodePtr() {
        return MapUtils<U, V>::getAllValues(nodePtrMap);
    }

    // Check if a key exists in the table
    bool containsLHS(const T& key) {
        return MapUtils<T, U>::contains(data, key);
    };

    // Check if a value exists in the table
    bool containsRHS(const U& value) {
        return MapUtils<U, T>::contains(reverseData, value);
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