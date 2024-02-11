#pragma once

#include <unordered_set>

// each entity will have its own table
// Can contain string which will represent variables, and procedure name
template<typename T>
class EntityManager {
    private:
        // Create a list which will contain numbers to store the statements that are of certain entity
        std::unordered_set<T> entityStmtNumSet = std::unordered_set<T>();

    public:
        void insert(T entity) {
            // Add the entity to the set
            entityStmtNumSet.insert(entity);
        };
        std::vector<T> get() {
            // Return the set
            // convert set to vector and return
            return std::vector<T>(entityStmtNumSet.begin(), entityStmtNumSet.end());
        };
        bool contains(T const & entity) {
            // Check if the entity is in the set
            return entityStmtNumSet.find(entity) != entityStmtNumSet.end();
        };
};
