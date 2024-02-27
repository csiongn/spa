#pragma once

#include <list>
#include <memory>

#include "PKB/facade/PKBFacade.h"
#include "PKB/Database.h"
#include "SP/facade/SPFacade.h"


class SPA {
private:
    std::shared_ptr<PKBFacade> pkbFacade;
    std::shared_ptr<Database> db;

public:
    SPA();
    void parseSimple(const std::string &program);
    void evaluateQuery(const std::string &query, std::list<std::string> &results);
};
