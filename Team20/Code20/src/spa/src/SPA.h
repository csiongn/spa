#pragma once

#include <list>
#include <memory>

#include "PKB/PKB.h"

class SPA {
private:
    std::shared_ptr<PKBFacade> pkb;

public:
    SPA();
    void parseSimple(const std::string &program);
    void evaluateQuery(const std::string &query, std::list<std::string> results);
};