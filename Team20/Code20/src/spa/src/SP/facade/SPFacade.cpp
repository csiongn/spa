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

void SPFacade::insertEntity(SimpleProgram::DesignEntity entity, const std::unordered_set<int>& value) {
    switch (entity) {
        case SimpleProgram::DesignEntity::ASSIGN:
            pkbFacade->insertAssign(value);
            break;
        case SimpleProgram::DesignEntity::CALL:
            pkbFacade->insertCall(value);
            break;
        case SimpleProgram::DesignEntity::STMT:
            pkbFacade->insertStatement(value);
            break;
        case SimpleProgram::DesignEntity::READ:
            pkbFacade->insertRead(value);
            break;
        case SimpleProgram::DesignEntity::PRINT:
            pkbFacade->insertPrint(value);
            break;
        case SimpleProgram::DesignEntity::WHILE:
            pkbFacade->insertWhile(value);
            break;
        case SimpleProgram::DesignEntity::IF:
            pkbFacade->insertIf(value);
            break;
        case SimpleProgram::DesignEntity::CONSTANT:
            pkbFacade->insertConstant(value);
            break;
        default:
            // TODO: Whether to throw error/exception
            std::cout << "Invalid entity type provided." << std::endl;
            break;
    }
}

void SPFacade::insertEntity(SimpleProgram::DesignEntity entity, const std::unordered_set<std::string>& value) {
    switch (entity) {
        case SimpleProgram::DesignEntity::VARIABLE:
            // insert string of set
            pkbFacade->insertVariable(value);
            break;
        case SimpleProgram::DesignEntity::PROCEDURE:
            std::cout << "Procedure entity type not supported yet." << std::endl;
            break;
        default:
            std::cout << "Invalid entity type provided." << std::endl;
            break;
    }
}

void SPFacade::insertRelationship(SimpleProgram::DesignAbstraction relationship, const std::unordered_map<int, std::unordered_set<int>>& map) {
    for (const auto& [key, value]: map) {
        insertRelationship(relationship,  key, value);
    }
}

void SPFacade::insertRelationship(SimpleProgram::DesignAbstraction relationship, const std::unordered_map<int, std::unordered_set<std::string>>& map) {
    for (const auto& [key, value]: map) {
        insertRelationship(relationship, key, value);
    }
}

void SPFacade::insertRelationship(SimpleProgram::DesignAbstraction relationship, int key, const std::unordered_set<int>& value) {
    switch (relationship) {
        case SimpleProgram::DesignAbstraction::FOLLOWS:
            pkbFacade->insertFollows(key, value);
            break;
        case SimpleProgram::DesignAbstraction::FOLLOWST:
            pkbFacade->insertFollowsT(key, value);
            break;
        case SimpleProgram::DesignAbstraction::PARENT:
            pkbFacade->insertParent(key, value);
            break;
        case SimpleProgram::DesignAbstraction::PARENTT:
            pkbFacade->insertParentT(key, value);
            break;
        default:
            std::cout << "Invalid relationship type provided." << std::endl;
            break;

    }
}

void SPFacade::insertRelationship(SimpleProgram::DesignAbstraction relationship, int key, const std::unordered_set<std::string>& value) {
    switch(relationship) {
        case SimpleProgram::DesignAbstraction::USES:
            pkbFacade->insertUsesStmt(key, value);
            break;
        case SimpleProgram::DesignAbstraction::MODIFIES:
            pkbFacade->insertModifiesStmt(key, value);
            break;
        default:
            std::cout << "Invalid relationship type provided." << std::endl;
            break;
    }
}


