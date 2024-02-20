#include "QPS/QPSManager.h"
#include "SPA.h"
#include <iostream>

SPA::SPA() {};

void SPA::parseSimple(const std::string &program) {
    // TODO: Change the initialization here
    db = std::make_shared<Database>();
    pkb = std::make_shared<PKBFacade>(db);

    SPFacade spFacade = SPFacade(pkb, program);
    spFacade.populatePKB();
}

void SPA::evaluateQuery(const std::string &query, std::list<std::string> &results) {
    QPSManager qpsManager = QPSManager(pkb);
    qpsManager.evaluate(query, results);
}