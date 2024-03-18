#pragma once

#include <list>
#include <memory>
#include <string>

#include "Evaluator/QueryEvaluator.h"
#include "PKB/facade/IPKBReader.h"
#include "QueryTokenizer.h"

class QPSManager {
  QueryTokenizer tokenizer;
  QueryEvaluator::QueryEvaluator evaluator;
  std::shared_ptr<IPKBReader> pkb;

 public:
  explicit QPSManager(std::shared_ptr<IPKBReader> pkb);

  void evaluate(const std::string &query, std::list<std::string> &results);
};
