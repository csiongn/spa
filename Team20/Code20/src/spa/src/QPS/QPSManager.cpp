#include "QPSManager.h"

QPSManager::QPSManager(std::shared_ptr<IPKBReader> pkb) : tokenizer(), evaluator(pkb) {};

void QPSManager::evaluate(const std::string &query, std::list<std::string> results) {
    std::vector<std::shared_ptr<QueryToken>> tokens = tokenizer.tokenize(query);

    // TODO parse

    // TODO change query to be from parser
    auto q = PQL::Query({}, {}, PQL::Synonym(SimpleProgram::DesignEntity::ASSIGN, "filler"));

    std::vector<std::string> res = evaluator.evaluateQuery(q);
    for (const auto &string : res) {
        results.push_back(string);
    }
}


