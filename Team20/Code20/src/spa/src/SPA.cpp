#include "QPS/QPSManager.h"
#include "SPA.h"
#include <iostream>

SPA::SPA() {};

void SPA::parseSimple(const std::string &program) {
    db = std::make_shared<Database>();
    pkbFacade = std::make_shared<PKBFacade>(db);
    SPFacade spFacade = SPFacade(pkbFacade, program);
    try {
        spFacade.populatePKB();
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred in parsing simple program, exiting with error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SPA::evaluateQuery(const std::string &query, std::list<std::string> &results) {
    QPSManager qpsManager = QPSManager(pkbFacade);
    try {
        qpsManager.evaluate(query, results);
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred in evaluating query: " << e.what() << std::endl;
    }
}