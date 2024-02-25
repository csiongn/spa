#pragma once

#include "ClauseEvaluator.h"

namespace QueryEvaluator {
    class AssignPatternEvaluator : private ClauseEvaluator {
    public:
        AssignPatternEvaluator(std::shared_ptr<IPKBReader> r, const PQL::Clause &cl,
                               std::shared_ptr<ResultStore> resultStore1)
                : ClauseEvaluator(r, cl, resultStore1) {};

        bool evaluate() override;

    private:
        bool isAlwaysFalse() override;

        bool hasRelationship() override;

        bool hasRelationship(const std::string &ident, const std::string &expr);

        bool hasAtLeastOneRelationship() override;

        bool getForwardRelationship() override;

        bool getReversedRelationship() override;

        bool getLeftResults() override;

        bool getSynonymWildcard() override;

        bool getRightResults() override;

        bool getWildcardSynonym() override;

        bool getDoubleSynonym() override;

        std::vector<std::shared_ptr<ExprNode>> getAllPartialNodes(const std::shared_ptr<ExprNode>& exprNode);

        std::vector<std::string> getAllAssignVar(const std::vector<std::shared_ptr<ExprNode>>& nodes);

        std::vector<int>
        getAssignSynResults(const std::vector<std::string>& vars, const std::vector<std::shared_ptr<ExprNode>>& exprNodes);
    };
}