#include <utility>

#include "QueryParser.h"
#include "QuerySemanticError.h"
#include "QuerySyntaxError.h"

QueryParser::QueryParser(std::vector<std::shared_ptr<QueryToken>> tokens) : tokens(std::move(tokens)), pos(0) {};

PQL::Query QueryParser::parse() {
    std::vector<PQL::Synonym> declarations;
    std::vector<PQL::Clause> clauses;

    declarations = parseDeclarations();
    PQL::Synonym selectSynonym = parseSelectClause(declarations);
    clauses = parseClauses(declarations);

    PQL::Query query = PQL::Query(declarations, clauses, selectSynonym);
    return query;
}


std::tuple<bool, SimpleProgram::DesignEntity> QueryParser::verifyDeclarationExists(const std::string& synonym, const std::vector<PQL::Synonym>& declarations) {
    for (const auto& syn : declarations) {
        if (syn.identity == synonym) {
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

bool QueryParser::isValidRelationshipArguments(int pos1, int pos2, const std::vector<PQL::Synonym>& declarations) {
    auto firstArgVerification = verifyDeclarationExists(tokens[pos1]->getValue(), declarations);
    bool isFirstArgValid = std::get<0>(firstArgVerification);

    auto secondArgVerification = verifyDeclarationExists(tokens[pos2]->getValue(), declarations);
    bool isSecondArgValid = std::get<0>(secondArgVerification);

    return isFirstArgValid && isSecondArgValid;
}

bool QueryParser::isValidAssignSynonym(std::shared_ptr<QueryToken>& token, const std::vector<PQL::Synonym>& declarations) {
    auto tokenValue = token->getValue();

    for (const auto& declaration : declarations) {
        if (declaration.entityType ==  SimpleProgram::DesignEntity::ASSIGN) {
            return declaration.identity == tokenValue;
        }
    }
    return false;
}

bool QueryParser::isValidPattern(int start, const std::vector<PQL::Synonym>& declarations) {
    auto assignSynonym = tokens[start++];
    bool isValidAssignSyn = isValidAssignSynonym(assignSynonym, declarations);
    bool hasOpeningBrace = tokens[start++]->getValue() == "(";
    bool hasValidFirstArgument = isEntRef(tokens[start++]);
    bool hasValidCommaSeparator = tokens[start++]->getValue() == ",";
    bool hasValidSecondArgument = isExpSpec(tokens[start++]);
    bool hasClosingBrace = tokens[start]->getValue() == ")";

    return true;
}

std::tuple<bool, SimpleProgram::DesignAbstraction, std::vector<PQL::Synonym>> QueryParser::getRelationshipAttributes(const std::vector<PQL::Synonym>& declarations) {
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
                bool areArgumentsValid = isValidRelationshipArguments(nextPos + 1, nextPos + 3, declarations);
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

PQL::Synonym QueryParser::createSynonym(std::shared_ptr<QueryToken>& token) {
    auto entityType = getEntityType(token);
    std::string id = token->getValue();
    PQL::Synonym synonym = PQL::Synonym(entityType, id);
    return synonym;
}

std::vector<PQL::Synonym> QueryParser::parseDeclarations() {
    std::vector<PQL::Synonym> declarations;

    int counter = 0;
    while (counter < tokens.size()) {
        if (counter + 2 >= tokens.size()) {
            pos = counter;
            break;
        }

        auto token1 = tokens[counter++];
        auto token2 = tokens[counter++];
        auto token3 = tokens[counter++];
        if (token1->getType() == TokenType::NAME && token2->getType() == TokenType::NAME && token3->getValue() == ";") {
            SimpleProgram::DesignEntity entityType = getEntityType(token1);
            const std::string id = token2->getValue();
            PQL::Synonym declaration = PQL::Synonym(entityType, id);
            declarations.push_back(declaration);
        } else {
            // declarations are of the form 'design_entity_type' 'id' ';'
            // if batch size > 3, no more declarations at this point.
            // at this point, we determine the position of the clauses in the token vector
            if (counter < tokens.size()) {
                pos = counter;
            }
            break;
        }
    }

    return declarations;
}

std::vector<PQL::Clause> QueryParser::parseClauses(const std::vector<PQL::Synonym>& declarations) {
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

            std::tuple<bool, SimpleProgram::DesignAbstraction, std::vector<PQL::Synonym>> verification = getRelationshipAttributes(declarations);
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

PQL::Synonym QueryParser::parseSelectClause(const std::vector<PQL::Synonym>& declarations) {
    auto currToken = tokens[pos];
    if (currToken->getValue() != "Select") {
        throw QuerySemanticError("Semantic Error: Select clause should come first");
    }

    auto selectSynonymToken = tokens[++pos];
    std::string id = selectSynonymToken->getValue();
    auto results = verifyDeclarationExists(id, declarations);
    bool isSynonymValid = std::get<0>(results);
    if (!isSynonymValid) {
        throw QuerySemanticError("Synonym used has not been declared");
    }
    auto entityType = std::get<1>(results);
    const PQL::Synonym selectSynonym = PQL::Synonym(entityType, id);
    pos += 2;
    return selectSynonym;
}