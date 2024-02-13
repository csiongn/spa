#pragma once

#include "SPFacade.h"

void SPFacade::populatePKB()
{
	// Tokenize program
	tokenizer.tokenizeProgram();
	const std::vector<Token>& tokens = tokenizer.getTokens();

	// Parse Tokens
	Parser ps = Parser(tokens);
	std::unique_ptr<ProgramNode> parsed_program = ps.parse();

	// Extract designs and populate PKB
	const ProgramNode& astRoot = *parsed_program;
	try {
		designExtractor.extractDesign(astRoot);
	}
	catch (const std::exception& e) {
		std::cout << "Exception caught: " << e.what() << std::endl;
	}

	// PKB is populated
}
