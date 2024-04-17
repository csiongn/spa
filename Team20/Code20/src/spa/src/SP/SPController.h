#pragma once

#include <unordered_map>
#include <string>
#include <unordered_set>
#include <memory>

#include "PKB/facade/IPKBWriter.h"
#include "SP/Tokenizer.h"
#include "SP/Parser.h"
#include "SP/DesignExtractor.h"
#include "Models/SimpleProgram.h"

class SPController {
 public:
  SPController(std::shared_ptr<IPKBWriter> pkbWriter, const std::string &input)
	  : designExtractor(pkbWriter),
		tokenizer(Tokenizer(input)),
		pkbWriter(pkbWriter) {}

  void populatePKB();

 private:
  DesignExtractor designExtractor;
  Tokenizer tokenizer;

 protected:
  std::shared_ptr<IPKBWriter> pkbWriter;
};