#include <memory>
#include <vector>
#include <string>

#include "QPSManager.h"
#include "QPS/Parser/QueryParser.h"
#include "QuerySemanticError.h"
#include "QuerySyntaxError.h"

QPSManager::QPSManager(std::shared_ptr<IPKBReader> pkb) : tokenizer(), evaluator(pkb) {};

void QPSManager::evaluate(const std::string &query, std::list<std::string> &results) {

  try {
	std::vector<std::shared_ptr<QueryToken>> tokens = tokenizer.tokenize(query);
	QueryParser parser(tokens);
	auto q = parser.parse();
	std::vector<std::string> res = evaluator.evaluateQuery(q);
	for (const auto &string : res) {
	  results.push_back(string);
	}
  } catch (const QuerySyntaxError &e) {
	results.emplace_back("SyntaxError");
  } catch (const QuerySemanticError &e) {
	results.emplace_back("SemanticError");
  } catch (const std::exception &e) {
	std::cerr << "Exception caught while evaluating query: " << e.what() << std::endl;
	// unknown error, does nothing
  } catch (...) {
	std::cerr << "Unknown exception caught while evaluating query." << std::endl;
  }
}


