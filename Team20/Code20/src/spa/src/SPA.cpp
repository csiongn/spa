#include "QPS/QPSManager.h"
#include "SPA.h"

SPA::SPA() : pkb() {};

void SPA::parseSimple(const std::string &program) {
    // TODO
}

void SPA::evaluateQuery(const std::string &query, std::list<std::string> results) {
    QPSManager qpsManager = QPSManager(pkb);
    qpsManager.evaluate(query, results);
}