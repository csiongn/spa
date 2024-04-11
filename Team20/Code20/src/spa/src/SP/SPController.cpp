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

	std::unique_ptr<CFGBuilder> cfgBuilder = std::make_unique<CFGBuilder>(designExtractor.getUses(), designExtractor.getModifies(), astRoot);
	std::shared_ptr<CFGManager> cfgManager = cfgBuilder->buildCFG();
	std::shared_ptr<NextExtractor> nextExtractor = std::make_shared<NextExtractor>();
  cfgManager->accept(*nextExtractor);
  nextExtractor->pushToPKB(pkbWriter);
	nextExtractor->finalize();

	std::shared_ptr<RuntimeExtractor> runtimeExtractor =
		std::make_shared<RuntimeExtractor>(cfgManager, nextExtractor);

	pkbWriter->insertRuntimeExtractor(runtimeExtractor);
}

