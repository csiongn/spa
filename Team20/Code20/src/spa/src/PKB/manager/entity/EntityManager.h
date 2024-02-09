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
            // Add the entity to the list
            entityStmtNumSet.insert(entity);
        };
        std::unordered_set<T> get() {
            // Return the list
            return entityStmtNumSet;
        };
        bool contains(T const & entity) {
            // Check if the entity is in the list
            return entityStmtNumSet.find(entity) != entityStmtNumSet.end();
        };
};
