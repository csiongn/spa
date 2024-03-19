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

  void insertEntity(SimpleProgram::DesignEntity entity, const std::unordered_set<int> &value);
  void insertEntity(SimpleProgram::DesignEntity entity, const std::unordered_set<std::string> &value);

  void insertRelationship(SimpleProgram::DesignAbstraction relationship,
						  const std::unordered_map<int, std::unordered_set<int>> &map);
  void insertRelationship(SimpleProgram::DesignAbstraction relationship, int key, const std::unordered_set<int> &value);
  void insertRelationship(SimpleProgram::DesignAbstraction relationship,
						  const std::unordered_map<int, std::unordered_set<std::string>> &map);
  void insertRelationship(SimpleProgram::DesignAbstraction relationship,
						  int key,
						  const std::unordered_set<std::string> &value);

 private:
  DesignExtractor designExtractor;
  Tokenizer tokenizer;

 protected:
  std::shared_ptr<IPKBWriter> pkbWriter;
};