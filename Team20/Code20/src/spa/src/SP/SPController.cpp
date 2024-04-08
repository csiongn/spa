#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <unordered_set>

#include "SPController.h"

#include "control_flow_graph/CFG.h"
#include "control_flow_graph/CFGBuilder.h"
#include "control_flow_graph/NextExtractor.h"
#include "RuntimeExtractor.h"

void SPController::populatePKB() {
  // Tokenize program
  tokenizer.tokenizeProgram();
  const std::vector<Token> &tokens = tokenizer.getTokens();

  // Parse Tokens
  Parser ps = Parser(tokens);
  std::shared_ptr<ProgramNode> parsed_program = ps.parse();

  // Extract designs and populate PKB
  const ProgramNode &astRoot = *parsed_program;
  designExtractor.extractDesign(astRoot);

	std::shared_ptr<CFGManager> cfgManager = CFGBuilder::buildCFG(astRoot);
	std::shared_ptr<NextExtractor> nextExtractor = std::make_shared<NextExtractor>();
  cfgManager->accept(*nextExtractor);
  nextExtractor->pushToPKB(pkbWriter);
	nextExtractor->finalize();

	std::shared_ptr<RuntimeExtractor> runtimeExtractor =
		std::make_shared<RuntimeExtractor>(cfgManager, nextExtractor);

	pkbWriter->insertRuntimeExtractor(runtimeExtractor);
}

void SPController::insertEntity(SimpleProgram::DesignEntity entity, const std::unordered_set<int> &value) {
  switch (entity) {
	case SimpleProgram::DesignEntity::ASSIGN:
	  pkbWriter->insertAssign(value);
	  break;
	case SimpleProgram::DesignEntity::CALL:
	  pkbWriter->insertCall(value);
	  break;
	case SimpleProgram::DesignEntity::STMT:
	  pkbWriter->insertStatement(value);
	  break;
	case SimpleProgram::DesignEntity::READ:
	  pkbWriter->insertRead(value);
	  break;
	case SimpleProgram::DesignEntity::PRINT:
	  pkbWriter->insertPrint(value);
	  break;
	case SimpleProgram::DesignEntity::WHILE:
	  pkbWriter->insertWhile(value);
	  break;
	case SimpleProgram::DesignEntity::IF:
	  pkbWriter->insertIf(value);
	  break;
	case SimpleProgram::DesignEntity::CONSTANT:
	  pkbWriter->insertConstant(value);
	  break;
	default:
	  // TODO: Whether to throw error/exception
	  std::cout << "Invalid entity type provided." << std::endl;
	  break;
  }
}

void SPController::insertEntity(SimpleProgram::DesignEntity entity, const std::unordered_set<std::string> &value) {
  switch (entity) {
	case SimpleProgram::DesignEntity::VARIABLE:
	  // insert string of set
	  pkbWriter->insertVariable(value);
	  break;
	case SimpleProgram::DesignEntity::PROCEDURE:
	  std::cout << "Procedure entity type not supported yet." << std::endl;
	  break;
	default:
	  std::cout << "Invalid entity type provided." << std::endl;
	  break;
  }
}

void SPController::insertRelationship(SimpleProgram::DesignAbstraction relationship,
									  const std::unordered_map<int, std::unordered_set<int>> &map) {
  for (const auto &[key, value] : map) {
	insertRelationship(relationship, key, value);
  }
}

void SPController::insertRelationship(SimpleProgram::DesignAbstraction relationship,
									  const std::unordered_map<int, std::unordered_set<std::string>> &map) {
  for (const auto &[key, value] : map) {
	insertRelationship(relationship, key, value);
  }
}

void SPController::insertRelationship(SimpleProgram::DesignAbstraction relationship,
									  int key,
									  const std::unordered_set<int> &value) {
  switch (relationship) {
	case SimpleProgram::DesignAbstraction::FOLLOWS:
	  pkbWriter->insertFollows(key, value);
	  break;
	case SimpleProgram::DesignAbstraction::FOLLOWST:
	  pkbWriter->insertFollowsT(key, value);
	  break;
	case SimpleProgram::DesignAbstraction::PARENT:
	  pkbWriter->insertParent(key, value);
	  break;
	case SimpleProgram::DesignAbstraction::PARENTT:
	  pkbWriter->insertParentT(key, value);
	  break;
	default:
	  std::cout << "Invalid relationship type provided." << std::endl;
	  break;
  }
}

void SPController::insertRelationship(SimpleProgram::DesignAbstraction relationship,
									  int key,
									  const std::unordered_set<std::string> &value) {
  switch (relationship) {
	case SimpleProgram::DesignAbstraction::USESS:
	  pkbWriter->insertUsesStmt(key, value);
	  break;
	case SimpleProgram::DesignAbstraction::MODIFIESS:
	  pkbWriter->insertModifiesStmt(key, value);
	  break;
	default:
	  std::cout << "Invalid relationship type provided." << std::endl;
	  break;
  }
}

