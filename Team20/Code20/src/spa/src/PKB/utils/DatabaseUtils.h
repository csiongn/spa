#pragma once

#include "PKB/manager/entity/EntityManager.h"
class DatabaseUtils {

public:
    // Entity
    template<typename T>
    static void insertEntity(std::shared_ptr<EntityManager<T>> manager, T entity) {
        manager->insert(entity);
    };

    template<typename T>
    static std::vector<T> getAllEntities(std::shared_ptr<EntityManager<T>> manager) {
        return manager->get();
    };

    template<typename T>
    static bool containsEntity(std::shared_ptr<EntityManager<T>> manager, T entity) {
        return manager->contains(entity);
    };

    // Relationship
    template<typename T, typename U>
    static void insertRelationship(std::shared_ptr<RelationshipManager<T, U>> manager, T first, U second) {
        manager->insert(first, second);
    };

    template<typename T, typename U>
    static bool containsKey(std::shared_ptr<RelationshipManager<T, U>> manager, T first) {
        return manager->contains(first);
    };

    template<typename T, typename U>
    static bool containsValue(std::shared_ptr<RelationshipManager<T, U>> manager, U second) {
        return manager->containsReverse(second);
    };

    template<typename T, typename U>
    static bool containsRelationship(std::shared_ptr<RelationshipManager<T, U>> manager, T first, U second) {
        return manager->containsRelationship(first, second);
    };

    template<typename T, typename U>
    static bool hasRelationship(std::shared_ptr<RelationshipManager<T, U>> manager) {
        return manager->hasRelationship();
    };

    template<typename T, typename U>
    static std::vector<U> getRelationship(std::shared_ptr<RelationshipManager<T, U>> manager, T first) {
        return manager->get(first);
    };

    template<typename T, typename U>
    static std::vector<T> getReverseRelationship(std::shared_ptr<RelationshipManager<T, U>> manager, U second) {
        return manager->getReverse(second);
    };

    template<typename T, typename U>
    static std::vector<T> getKeys(std::shared_ptr<RelationshipManager<T, U>> manager) {
        return manager->getKeys();
    };

    template<typename T, typename U>
    static std::vector<U> getValues(std::shared_ptr<RelationshipManager<T, U>> manager) {
        return manager->getValues();
    };

    // Assign Pattern
    template<typename T, typename U, typename V>
    static void insertPattern(std::shared_ptr<PatternManager<T, U, V>> manager, T lhsVar, U rhsExprNodeHash, int stmtNum, V nodePtr) {
        manager->insert(lhsVar, rhsExprNodeHash, stmtNum, nodePtr);
    };

    template<typename T, typename U, typename V>
    static std::vector<T> getPatternLHS(std::shared_ptr<PatternManager<T, U, V>> manager, U rhsExprNodeHash) {
        return manager->getLHS(rhsExprNodeHash);
    };

    template<typename T, typename U, typename V>
    static std::vector<T> getAllPatternLHS(std::shared_ptr<PatternManager<T, U, V>> manager) {
        return manager->getLHSKeys();
    };

    template<typename T, typename U, typename V>
    static std::vector<U> getPatternRHS(std::shared_ptr<PatternManager<T, U, V>> manager, T lhsVar) {
        return manager->getRHS(lhsVar);
    };

    template<typename T, typename U, typename V>
    static std::vector<U> getAllPatternRHS(std::shared_ptr<PatternManager<T, U, V>> manager) {
        return manager->getRHSKeys();
    };

    template<typename T, typename U, typename V>
    static std::vector<int> getPatternStmtNum(std::shared_ptr<PatternManager<T, U, V>> manager, T lhsVar, U rhsExprNodeHash) {
        return manager->getPatternStmtNum(lhsVar, rhsExprNodeHash);
    };

    template<typename T, typename U, typename V>
    static std::vector<int> getPatternLHSStmtNum(std::shared_ptr<PatternManager<T, U, V>> manager, T lhsVar) {
        return manager->getLHSStmtNum(lhsVar);
    };

    template<typename T, typename U, typename V>
    static std::vector<int> getPatternRHSStmtNum(std::shared_ptr<PatternManager<T, U, V>> manager, U rhsExprNodeHash) {
        return manager->getRHSStmtNum(rhsExprNodeHash);
    };

    template<typename T, typename U, typename V>
    static std::vector<V> getPatternRHSNodePtr(std::shared_ptr<PatternManager<T, U, V>> manager, U rhsExprNodeHash) {
        return manager->getRHSNodePtr(rhsExprNodeHash);
    };

    template<typename T, typename U, typename V>
    static std::vector<V> getPatternRHSNodePtr(std::shared_ptr<PatternManager<T, U, V>> manager) {
        return manager->getRHSNodePtr();
    };

    template<typename T, typename U, typename V>
    static bool containsPattern(std::shared_ptr<PatternManager<T, U, V>> manager, T lhsVar, U rhsExprNodeHash) {
        return manager->containsPattern(lhsVar, rhsExprNodeHash);
    };

    template<typename T, typename U, typename V>
    static bool containsPatternLHS(std::shared_ptr<PatternManager<T, U, V>> manager, T lhsVar) {
        return manager->containsLHS(lhsVar);
    };

    template<typename T, typename U, typename V>
    static bool containsPatternRHS(std::shared_ptr<PatternManager<T, U, V>> manager, U rhsExprNodeHash) {
        return manager->containsRHS(rhsExprNodeHash);
    };

    template<typename T, typename U, typename V>
    static bool hasPattern(std::shared_ptr<PatternManager<T, U, V>> manager) {
        return manager->hasPattern();
    };

    // Conditional Patterns
    template<typename T, typename U>
    static void insertConditionalPattern(std::shared_ptr<ConditionalPatternManager<T, U>> manager, T key, U value) {
        manager->insert(key, value);
    };

    template<typename T, typename U>
    static std::vector<U> getConditionalPatternValues(
            std::shared_ptr<ConditionalPatternManager<T, U>> manager, T key) {
        return manager->get(key);
    };

    template<typename T, typename U>
    static std::vector<U> getConditionalPatternValues(
            std::shared_ptr<ConditionalPatternManager<T, U>> manager) {
        return manager->getValues();
    };

    template<typename T, typename U>
    static std::vector<T> getConditionalPatternKeys(
            std::shared_ptr<ConditionalPatternManager<T, U>> manager, U value) {
        return manager->getReverse(value);
    };

    template<typename T, typename U>
    static std::vector<T> getConditionalPatternKeys(
            std::shared_ptr<ConditionalPatternManager<T, U>> manager) {
        return manager->getKeys();
    };

    template<typename T, typename U>
    static bool containsConditionalPattern(std::shared_ptr<ConditionalPatternManager<T, U>> manager, T key, U value) {
        return manager->containsPattern(key, value);
    };

    template<typename T, typename U>
    static bool containsConditionalPatternKey(std::shared_ptr<ConditionalPatternManager<T, U>> manager, T key) {
        return manager->contains(key);
    };

    template<typename T, typename U>
    static bool containsConditionalPatternValue(std::shared_ptr<ConditionalPatternManager<T, U>> manager, U value) {
        return manager->containsReverse(value);
    };

    template<typename T, typename U>
    static bool hasConditionalPattern(std::shared_ptr<ConditionalPatternManager<T, U>> manager) {
        return manager->hasPattern();
    };

};