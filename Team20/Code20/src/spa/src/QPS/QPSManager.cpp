#include "QPSManager.h"
#include "QueryParser.h"
#include "QuerySemanticError.h"
#include "QuerySyntaxError.h"

QPSManager::QPSManager(std::shared_ptr<IPKBReader> pkb) : tokenizer(), evaluator(pkb) {};

void QPSManager::evaluate(const std::string &query, std::list<std::string> &results) {
    std::vector<std::shared_ptr<QueryToken>> tokens = tokenizer.tokenize(query);
    QueryParser parser(tokens);

    try {
        auto q = parser.parse();
        std::vector<std::string> res = evaluator.evaluateQuery(q);
        for (const auto &string : res) {
            results.push_back(string);
        }
    } catch (const QuerySyntaxError &e){
        results.emplace_back(e.what());
    } catch (const QuerySemanticError &e) {
        results.emplace_back(e.what());
    } catch (...) {
        // unknown error, does nothing
    }
}


