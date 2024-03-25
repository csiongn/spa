#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "ICFGVisitor.h"

class NextExtractor : public ICFGVisitor {
    std::unordered_map<int, std::vector<int>> nextRelationships;
    std::set<int> visited;

 public:
    void visit(const std::shared_ptr<CFGNode>& node) override;
    void pushToPKB(const std::shared_ptr<IPKBWriter> &pkbWriter) override;
    const std::unordered_map<int, std::vector<int>>& getNextRelationships() const;
};
