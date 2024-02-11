# pragma once

# include <unordered_map>
# include <string>
# include <memory>
# include <unordered_set>
# include <vector>


/*
The method is implemented in a template class in a .cpp file: If RelationshipManager is a template class and its implementation is in a .cpp file,
you will run into issues because templates in C++ need to have their implementation in the same file as their declaration (usually in the header file),
or in a file that is included in the header file.
This is because templates are compiled when they are instantiated,
so the compiler needs to have access to the full template definition, not just the declaration.
*/
template <typename T, typename U>
class RelationshipManager {

    // Only accessible by derived classes
    private:
        std::unordered_map<T, std::unordered_set<U> > data;
        std::unordered_map<U, std::unordered_set<T> > reverseData;

    public:

        // Insert a record into the table
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

        // Check if a key exists in the table
        bool contains(const T& key) {
            return data.find(key) != data.end();
        };

        // Check if a value exists in the table
        bool containsReverse(const U& value) {
            return reverseData.find(value) != reverseData.end();
        };

        bool containsValueInKeySet(const T& key, const U& value) {
            if (contains(key)) {
                const auto &valueSet= data[key];
                return (valueSet.find(value) != valueSet.end());
            }
            return false; // key does not exist
        }


};