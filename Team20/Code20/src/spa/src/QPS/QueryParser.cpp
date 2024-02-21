#include <utility>

#include "QueryParser.h"
#include "QuerySemanticError.h"
#include "QuerySyntaxError.h"

QueryParser::QueryParser(std::vector<std::shared_ptr<QueryToken>> tokens) : tokens(std::move(tokens)), pos(0) {};

PQL::Query QueryParser::parse() {
    std::vector<PQL::Clause> clauses;

    initialDeclarations = parseDeclarations();
    PQL::Synonym selectSynonym = parseSelectClause();
    clauses = parseClauses();

    PQL::Query query = PQL::Query(usedDeclarations, clauses, selectSynonym);
    return query;
}


std::tuple<bool, SimpleProgram::DesignEntity> QueryParser::verifyDeclarationExists(const std::shared_ptr<QueryToken>& token) {
    std::string tokenId = token->getValue();
    if (token->getType() == TokenType::INTEGER) {
        return std::make_tuple(true, SimpleProgram::DesignEntity::STMT_NO);
    }
    for (const auto& syn : initialDeclarations) {
        if (syn.identity == tokenId) {
            // Populate usedDeclarations array
            usedDeclarations.push_back(syn);
            return std::make_tuple(true, syn.entityType);
        }
    }
    return std::make_tuple(false, SimpleProgram::DesignEntity{});
}

bool QueryParser::isStmtRef(std::shared_ptr<QueryToken>& token) {
    return token->getType() == TokenType::INTEGER  ||
        token->getType() == TokenType::NAME ||
        token->getType() == TokenType::WILDCARD;
}

bool QueryParser::isEntRef(std::shared_ptr<QueryToken>& token) {
    return token->getType() == TokenType::NAME ||
        token->getType() == TokenType::WILDCARD ||
        token->getType() == TokenType::CONSTANT_STRING;
}

bool QueryParser::isExpSpec(std::shared_ptr<QueryToken>& token) {
    // TODO: Implement isExpSpec
    return false;
}

bool QueryParser::isValidRelationship(int start) {
    bool hasOpeningBrace = tokens[start++]->getValue() == "(";
    bool hasValidFirstArgument = isStmtRef(tokens[start++]);
    bool hasValidCommaSeparator = tokens[start++]->getValue() == ",";
    bool hasValidSecondArgument = isStmtRef(tokens[start++]);
    bool hasClosingBrace = tokens[start]->getValue() == ")";

    if (!hasValidFirstArgument || !hasValidSecondArgument) {
        throw QuerySemanticError("Semantic Error on arguments: should be in the form of a stmtRef");
    }

    return hasOpeningBrace && hasValidCommaSeparator && hasClosingBrace;
}

bool QueryParser::isValidRelationshipArguments(int pos1, int pos2) {
    auto firstArgVerification = verifyDeclarationExists(tokens[pos1]);
    bool isFirstArgValid = std::get<0>(firstArgVerification);

    auto secondArgVerification = verifyDeclarationExists(tokens[pos2]);
    bool isSecondArgValid = std::get<0>(secondArgVerification);

    return isFirstArgValid && isSecondArgValid;
}

bool QueryParser::isValidAssignSynonym(std::shared_ptr<QueryToken>& token) {
    auto tokenValue = token->getValue();

    for (const auto& declaration : initialDeclarations) {
        if (declaration.entityType ==  SimpleProgram::DesignEntity::ASSIGN) {
            return declaration.identity == tokenValue;
        }
    }
    return false;
}

bool QueryParser::isValidPattern(int start) {
    auto assignSynonym = tokens[start++];
    bool isValidAssignSyn = isValidAssignSynonym(assignSynonym);
    bool hasOpeningBrace = tokens[start++]->getValue() == "(";
    bool hasValidFirstArgument = isEntRef(tokens[start++]);
    bool hasValidCommaSeparator = tokens[start++]->getValue() == ",";
    bool hasValidSecondArgument = isExpSpec(tokens[start++]);
    bool hasClosingBrace = tokens[start]->getValue() == ")";

    return true;
}

std::tuple<bool, SimpleProgram::DesignAbstraction, std::vector<PQL::Synonym>> QueryParser::getRelationshipAttributes() {
    // all relationships will take on the following format:
    // 'such that' {relRef}
    auto currToken = tokens[pos];
    std::string tokenValue = currToken->getValue();

    int nextPos = pos + 1;
    if (tokenValue == "Follows" || tokenValue == "Parent") {
        // Follows(stmtRef, stmtRef)
        // Parent(stmtRef, stmtRef)

        SimpleProgram::DesignAbstraction abstraction;
        std::vector<PQL::Synonym> args;

        if (nextPos < tokens.size()) {
            auto nextToken = tokens[nextPos];
            std::string nextTokenValue = nextToken->getValue();

            // check if is Follows* or Parent*
            if (nextTokenValue == "*") {
                nextPos++;
                abstraction = tokenValue == "Follows" ? SimpleProgram::DesignAbstraction::FOLLOWST : SimpleProgram::DesignAbstraction::PARENTT;
            } else {
                abstraction = tokenValue == "Follows" ? SimpleProgram::DesignAbstraction::FOLLOWS : SimpleProgram::DesignAbstraction::PARENT;
            }

            // total of 5 more tokens: '(' 'stmtRef' ',' 'stmtRef' ')'
            if (nextPos + 4 >= tokens.size()) {
                throw QuerySyntaxError("Syntax Error occurred: Relationship has wrong format");
            }

            bool isValidRs = isValidRelationship(nextPos);

            if (isValidRs) {
                bool areArgumentsValid = isValidRelationshipArguments(nextPos + 1, nextPos + 3);
                if (areArgumentsValid) {
                    auto arg1 = createSynonym(tokens[nextPos + 1]);
                    auto arg2 = createSynonym(tokens[nextPos + 3]);
                    args.push_back(arg1);
                    args.push_back(arg2);
                    pos = nextPos + 5;
                } else {
                    throw QuerySemanticError("Semantic Error on arguments: arguments used not declared");
                }
            }

            return std::make_tuple(isValidRs, isValidRs ? abstraction : SimpleProgram::DesignAbstraction{}, args);

        } else {
            throw QuerySyntaxError("Syntax Error occurred: Relationship has wrong format");
        }

    } else {
        // TODO: additional support for Uses and Modifies to be added later
    }
}

SimpleProgram::DesignEntity QueryParser::getEntityType(const std::shared_ptr<QueryToken>& token) {
    std::string tokenValue = token->getValue();
    if (tokenValue == "procedure") {
        return SimpleProgram::DesignEntity::PROCEDURE;
    } else if (tokenValue == "read") {
        return SimpleProgram::DesignEntity::READ;
    } else if (tokenValue == "stmt") {
        return SimpleProgram::DesignEntity::STMT;
    } else if (tokenValue == "print") {
        return SimpleProgram::DesignEntity::PRINT;
    } else if (tokenValue == "assign") {
        return SimpleProgram::DesignEntity::ASSIGN;
    } else if (tokenValue == "call") {
        return SimpleProgram::DesignEntity::CALL;
    } else if (tokenValue == "while") {
        return SimpleProgram::DesignEntity::WHILE;
    } else if (tokenValue == "if") {
        return SimpleProgram::DesignEntity::IF;
    } else if (tokenValue == "variable") {
        return SimpleProgram::DesignEntity::VARIABLE;
    } else if (tokenValue == "constant") {
        return SimpleProgram::DesignEntity::CONSTANT;
    } else {
        throw QuerySyntaxError("Syntax Error for the following token: " + tokenValue);
    }
}

SimpleProgram::DesignEntity QueryParser::getEntityTypeFromSynonym(const std::shared_ptr<QueryToken>& token) {
    if (token->getType() == TokenType::INTEGER) {
        return SimpleProgram::DesignEntity::STMT_NO;
    }

    for (const auto& dec : initialDeclarations) {
        std::string currTokenId = token->getValue();
        if (dec.identity == currTokenId) {
            return dec.entityType;
        }
    }
    return SimpleProgram::DesignEntity{};
}

PQL::Synonym QueryParser::createSynonym(std::shared_ptr<QueryToken>& token) {
    auto entityType = getEntityTypeFromSynonym(token);
    std::string id = token->getValue();
    PQL::Synonym synonym = PQL::Synonym(entityType, id);
    return synonym;
}

std::vector<PQL::Synonym> QueryParser::parseDeclarations() {
    std::vector<PQL::Synonym> declarations;

    std::vector<std::shared_ptr<QueryToken>> currDeclarations;
    while (pos < tokens.size()) {
        auto currToken = tokens[pos];

        if (currToken->getValue() == "Select") {
            break;
        }

        if (currToken->getValue() == ";") {
            // Add all the declarations in the array.
            // First element should always be the type, following elements will be the synonyms used
            auto entityType = getEntityType(currDeclarations[0]);
            for (int i = 1; i < currDeclarations.size(); i++) {
                auto currIdentity = currDeclarations[i]->getValue();
                auto declaration = PQL::Synonym(entityType, currIdentity);
                declarations.push_back(declaration);
            }
            currDeclarations.clear();
            pos++;
            continue;
        }

        if (currToken->getValue() == ",") {
            pos++;
            continue;
        }

        currDeclarations.push_back(currToken);
        pos++;
    }
    return declarations;
}

std::vector<PQL::Clause> QueryParser::parseClauses() {
    std::vector<PQL::Clause> clauses;

    while (pos < tokens.size()) {
        auto currToken = tokens[pos];
        std::string tokenValue = currToken->getValue();

        // only have two options, either 'such that' clause, or 'pattern' clause

        if (tokenValue == "pattern") {
            // TODO: Add implementation for 'pattern' clause
        } else {
            if (pos + 1 < tokens.size()) {
                auto nextToken = tokens[pos++];
                auto nextTokenValue = nextToken->getValue();

                if (tokenValue != "such" && nextTokenValue != "that") {
                    continue;
                }
            }

            std::tuple<bool, SimpleProgram::DesignAbstraction, std::vector<PQL::Synonym>> verification = getRelationshipAttributes();
            // pos will be incremented in getRelationshipAttributes;
            bool isVerified = std::get<0>(verification);
            SimpleProgram::DesignAbstraction abstraction = std::get<1>(verification);
            std::vector<PQL::Synonym> args = std::get<2>(verification);

            if (isVerified) {
                auto clause = PQL::Clause(abstraction, args);
                clauses.push_back(clause);
            }
        }
    }

    return clauses;
}

PQL::Synonym QueryParser::parseSelectClause() {
    auto currToken = tokens[pos];
    if (currToken->getValue() != "Select") {
        throw QuerySemanticError("Semantic Error: Select clause should come first");
    }

    auto selectSynonymToken = tokens[++pos];
    std::string id = selectSynonymToken->getValue();
    auto results = verifyDeclarationExists(selectSynonymToken);
    bool isSynonymValid = std::get<0>(results);
    if (!isSynonymValid) {
        throw QuerySemanticError("Synonym used has not been declared");
    }
    auto entityType = std::get<1>(results);
    const PQL::Synonym selectSynonym = PQL::Synonym(entityType, id);
    pos += 2;
    return selectSynonym;
}