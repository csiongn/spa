# pragma once

# include <unordered_map>
# include <string>
# include <memory>
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
        // E.g: Follows or Followed relationship
        // map of key to vector of values
        // Statements that follow 1 directly
        // E.g: Follows(1, s) -> Follower[1] = [2, 3]
        std::unordered_map<T, std::vector<U> > data;
        // E.g: Follows(1, 2) -> Following[2] = [1]
        std::unordered_map<U, std::vector<T> > reverseData;

    // WHERE DO I ADD VIRTUAL FUNCTIONS?
    public:

        // Insert a record into the table
        void insert(const T& key, const U& value) {
            data[key].push_back(value);
            reverseData[value].push_back(key);
        };

        // Get the value associated with a key
        std::vector<U> get(const T &key) {
            if (data.find(key) == data.end()) {
                return std::vector<U>();
            }
            return data.at(key);
        };

        // Get the key associated with a value
        std::vector<T> getReverse(const U& value) {
            if (reverseData.find(value) == reverseData.end()) {
                return std::vector<T>();
            }
            return reverseData.at(value);
        };

        // Check if a key exists in the table
        bool contains(const T& key) {
            return data.find(key) != data.end();
        };

        // Check if a value exists in the table
        bool containsReverse(const U& value) {
            return reverseData.find(value) != reverseData.end();
        };;


};