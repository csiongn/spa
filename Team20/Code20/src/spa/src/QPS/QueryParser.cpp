#include <utility>

#include "QueryParser.h"
#include "QuerySemanticError.h"
#include "QuerySyntaxError.h"

QueryParser::QueryParser(std::vector<std::shared_ptr<QueryToken>> tokens) : tokens(std::move(tokens)), pos(0) {}

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

    if (token->getType() == QPS::TokenType::WILDCARD) {
        return std::make_tuple(true, SimpleProgram::DesignEntity::WILDCARD);
    }

    if (token->getType() == QPS::TokenType::CONSTANT_STRING) {
        return std::make_tuple(true, SimpleProgram::DesignEntity::IDENT);
    }

    for (const auto& syn : initialDeclarations) {
        if (syn.identity == tokenId) {
            // Populate usedDeclarations array if not already in array
            bool usedDeclarationExists = std::find(std::begin(usedDeclarations), std::end(usedDeclarations), syn) != std::end(usedDeclarations);
            if (!usedDeclarationExists) {
                usedDeclarations.push_back(syn);
            }
            return std::make_tuple(true, syn.entityType);
        }
    }

    if (token->getType() == QPS::TokenType::INTEGER) {
        return std::make_tuple(true, SimpleProgram::DesignEntity::STMT_NO);
    }

    return std::make_tuple(false, SimpleProgram::DesignEntity{});
}

bool QueryParser::verifyNoDuplicateDeclarations(const std::string synonymIdentity) {
    for (const auto& syn : initialDeclarations) {
        // We are only concerned about the identity here, not the entityType
        if (syn.identity == synonymIdentity) {
            return true;
        }
    }
    return false;
}

bool QueryParser::isStmtRef(std::shared_ptr<QueryToken>& token) {
    return token->getType() == QPS::TokenType::INTEGER  ||
        token->getType() == QPS::TokenType::NAME ||
        token->getType() == QPS::TokenType::WILDCARD;
}

bool QueryParser::isEntRef(std::shared_ptr<QueryToken>& token) {
    return token->getType() == QPS::TokenType::NAME ||
        token->getType() == QPS::TokenType::WILDCARD ||
            (token->getType() == QPS::TokenType::CONSTANT_STRING && isName(token->getValue()));
}

bool QueryParser::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool QueryParser::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool QueryParser::isName(const std::string& str) {
    if (str.empty()) {
        return false; // Empty string is not a valid name
    }

    // Check if the first character is a letter
    if (!isLetter(str[0])) {
        return false;
    }

    // Check if the remaining characters are letters or digits
    for (size_t i = 1; i < str.size(); ++i) {
        if (!isLetter(str[i]) && !isDigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool QueryParser::isInteger(const std::string& str) {
    if (str.empty()) {
        return false; // Empty string is not a valid integer
    }

    // If the first character is '0', it must be the only character in the string
    if (str.size() == 1 && str[0] == '0') {
        return true;
    }

    // If the first character is not '0', it must be a non-zero digit
    if (str[0] < '1' || str[0] > '9') {
        return false;
    }

    // Check if the remaining characters are digits
    for (size_t i = 1; i < str.size(); ++i) {
        if (!isDigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool QueryParser::isExpSpec(const std::vector<std::shared_ptr<QueryToken>>& expSpecTokens) {
    if (expSpecTokens.empty()) {
        return false;
    } else if (expSpecTokens.size() == 1 ) {
        return expSpecTokens[0]->getType() == QPS::TokenType::WILDCARD || expSpecTokens[0]->getType() == QPS::TokenType::CONSTANT_STRING && isFactor(expSpecTokens[0]);
    } else if (expSpecTokens.size() == 3) {
        return expSpecTokens[0]->getType() == QPS::TokenType::WILDCARD && expSpecTokens[2]->getType()==QPS::TokenType::WILDCARD && isFactor(expSpecTokens[1]);
    } else {
        throw QuerySyntaxError("Syntax Error: Invalid expression syntax");
    }
}


bool QueryParser::isFactor(const std::shared_ptr<QueryToken>& token) {
    bool isFactor = isName(token->getValue()) || isInteger(token->getValue());
    return isFactor;
}

bool QueryParser::isStmtSubtype(std::shared_ptr<QueryToken>& token) {
    auto tokenEntityType = getEntityType(token);
    if (tokenEntityType == SimpleProgram::DesignEntity::STMT || tokenEntityType == SimpleProgram::DesignEntity::READ || tokenEntityType == SimpleProgram::DesignEntity::PRINT || tokenEntityType == SimpleProgram::DesignEntity::ASSIGN || tokenEntityType == SimpleProgram::DesignEntity::CALL) {
        return true;
    }
    return false;
}

bool QueryParser::isValidRelationship(int start, SimpleProgram::DesignAbstraction relationship) {
    bool isFollowsOrParent = relationship == SimpleProgram::DesignAbstraction::FOLLOWS ||
            relationship == SimpleProgram::DesignAbstraction::FOLLOWST ||
            relationship == SimpleProgram::DesignAbstraction::PARENT ||
            relationship == SimpleProgram::DesignAbstraction::PARENTT;
    bool isUses = relationship == SimpleProgram::DesignAbstraction::USESS;
    bool isModifies = relationship == SimpleProgram::DesignAbstraction::MODIFIESS;

    bool hasOpeningBrace = tokens[start++]->getValue() == "(";
    bool hasValidFirstArgument = isStmtRef(tokens[start++]);

    if (hasValidFirstArgument && tokens[start-1]->getType() == QPS::TokenType::NAME) {
        auto declarationVerification = verifyDeclarationExists(tokens[start-1]);
        auto synonymType = std::get<1>(declarationVerification);
        // Check synonym types used
        bool isAllowedType = synonymType == SimpleProgram::DesignEntity::ASSIGN ||
                             synonymType == SimpleProgram::DesignEntity::STMT ||
                             synonymType == SimpleProgram::DesignEntity::CALL ||
                             synonymType == SimpleProgram::DesignEntity::WHILE ||
                             synonymType == SimpleProgram::DesignEntity::IF ||
                             synonymType == SimpleProgram::DesignEntity::PROCEDURE ||
                             synonymType == SimpleProgram::DesignEntity::READ ||
                             synonymType == SimpleProgram::DesignEntity::PRINT;
        if (!isAllowedType) {
            throw QuerySemanticError("Semantic Error: Synonym type used for first argument is not allowed");
        }
    }

    if (!isFollowsOrParent && hasValidFirstArgument) {
        if (tokens[start - 1]->getType() == QPS::TokenType::WILDCARD) {
            throw QuerySemanticError("Semantic Error: First argument to Modifies or Uses cannot be _");
        }
    }


    auto firstArgToken = tokens[start-1];

    if (isFollowsOrParent && hasValidFirstArgument && firstArgToken->getType() != QPS::TokenType::NAME && firstArgToken->getType() != QPS::TokenType::INTEGER && firstArgToken->getType() != QPS::TokenType::WILDCARD && !isName(firstArgToken->getValue())) {

        if (!isStmtSubtype(tokens[start-1])) {
            throw QuerySemanticError("Semantic Error: first argument must be a statement synonym, or a subtype of a statement synonym (read, print, assign, if, while, call)");
        }
    }

    bool hasValidCommaSeparator = tokens[start++]->getValue() == ",";
    bool hasValidSecondArgument;
    auto secondArgToken = tokens[start++];

    if (isFollowsOrParent) {
        hasValidSecondArgument = isStmtRef(secondArgToken);
    }

    if (!isFollowsOrParent) {
        hasValidSecondArgument = isEntRef(secondArgToken);
        if (hasValidSecondArgument && secondArgToken->getType() != QPS::TokenType::CONSTANT_STRING && isName(secondArgToken->getValue()) && getEntityTypeFromSynonym(secondArgToken) != SimpleProgram::DesignEntity::VARIABLE) {

            throw QuerySemanticError("Semantic Error: Second argument to Modifies and Uses should be a variable synonym");
        }
    }

    if (hasValidSecondArgument && isName(tokens[start-1]->getValue())) {
        if (!std::get<0>(verifyDeclarationExists(tokens[start-1]))) {
            throw QuerySemanticError("Semantic Error: Declaration does not exist");
        }
    }

    bool hasClosingBrace = tokens[start]->getValue() == ")";

    if (!hasValidFirstArgument || !hasValidSecondArgument) {
        throw QuerySyntaxError("Syntax Error: should be in the form of a stmtRef");
    }

    return hasOpeningBrace && hasValidCommaSeparator && hasClosingBrace;
}

bool QueryParser::isValidRelationshipArguments(int pos1, int pos2, bool isFollowsOrParent) {
    auto firstArgVerification = verifyDeclarationExists(tokens[pos1]);
    bool isFirstArgValid = std::get<0>(firstArgVerification);

    std::tuple<bool, SimpleProgram::DesignEntity> secondArgVerification;
    bool isSecondArgValid = true;

    if (isFollowsOrParent) {
        secondArgVerification = verifyDeclarationExists(tokens[pos2]);
        isSecondArgValid = std::get<0>(secondArgVerification);
    }

    if (!isFollowsOrParent && tokens[pos2]->getType() != QPS::TokenType::CONSTANT_STRING) {
        secondArgVerification = verifyDeclarationExists(tokens[pos2]);
        isSecondArgValid = std::get<0>(secondArgVerification);
    }

    return isFirstArgValid && isSecondArgValid;
}

bool QueryParser::isValidPatternSynonym(std::shared_ptr<QueryToken>& token) {
    auto tokenValue = token->getValue();

    bool isValid = false;
    for (const auto& declaration : initialDeclarations) {
        if (isValid) {
            break;
        }

        const bool isAllowedPatternSynonyms = declaration.entityType == SimpleProgram::DesignEntity::ASSIGN
                || declaration.entityType == SimpleProgram::DesignEntity::IF
                || declaration.entityType == SimpleProgram::DesignEntity::WHILE;
        if (isAllowedPatternSynonyms) {
            isValid = isValid || declaration.identity == tokenValue;
            bool usedDeclarationExists = std::find(std::begin(usedDeclarations), std::end(usedDeclarations), declaration) != std::end(usedDeclarations);
            if (isValid && !usedDeclarationExists) {
                usedDeclarations.push_back(declaration);
            }
        }
    }
    return isValid;
}

bool QueryParser::isValidPattern(int start, int end) {
    if (start > end) {
        throw QuerySyntaxError("Syntax Error: Invalid pattern");
    }

    // pattern syntax:
    // 'pattern' {syn-assign} '(' {entRef} ',' {expSpec} ')'
    // 'pattern' {syn-ifs} '(' {entRef} ',' '_' ',' '_' ')'
    // 'pattern' {syn-while} '(' {entRef} ',' '_' ')'
    if (end >= tokens.size()) {
        throw QuerySyntaxError("Syntax Error: Pattern clause has wrong format");
    }
    auto patternSynonym = tokens[start++];
    SimpleProgram::DesignEntity patternSynonymEntityType;

    bool isValidPatternSyn = isValidPatternSynonym(patternSynonym);
    if (isValidPatternSyn) {
        patternSynonymEntityType = getEntityTypeFromSynonym(patternSynonym);
    }

    bool hasOpeningBrace = tokens[start++]->getValue() == "(";
    // All pattern clause allows entRef as first argument for language rules
    bool hasValidFirstArgument = isEntRef(tokens[start++]);

    if (hasValidFirstArgument && tokens[start-1]->getType() == QPS::TokenType::NAME) {
        auto isValidSynonym = verifyDeclarationExists(tokens[start-1]);
        if (std::get<1>(isValidSynonym) != SimpleProgram::DesignEntity::VARIABLE) {
            throw QuerySemanticError("Semantic Error: Synonym for first argument should be a variable synonym");
        }
    }

    bool hasValidCommaSeparator = tokens[start++]->getValue() == ",";

    bool hasValidSecondArgument = true;
    bool hasValidSecondCommaSeparator = true;
    bool hasValidThirdArgument = true;

    if (patternSynonymEntityType == SimpleProgram::DesignEntity::ASSIGN) {
        std::vector<std::shared_ptr<QueryToken>> expSpecTokens;
        for (int i = start; i < end; i++) {
            expSpecTokens.push_back(tokens[i]);
        }

        hasValidSecondArgument = isExpSpec(expSpecTokens);
    } else if (patternSynonymEntityType == SimpleProgram::DesignEntity::IF) {
        hasValidSecondArgument = tokens[start++]->getType() == QPS::TokenType::WILDCARD;
        hasValidSecondCommaSeparator = tokens[start++]->getValue() == ",";
        hasValidThirdArgument = tokens[start++]->getType() == QPS::TokenType::WILDCARD;
    } else if (patternSynonymEntityType == SimpleProgram::DesignEntity::WHILE) {
        hasValidSecondArgument = tokens[start++]->getType() == QPS::TokenType::WILDCARD;
    } else {
        throw QuerySyntaxError("Syntax Error: Invalid pattern clause");
    }


    bool hasClosingBrace = tokens[end]->getValue() == ")";

    bool isValidPatternRes = isValidPatternSyn && hasOpeningBrace && hasValidFirstArgument
            && hasValidCommaSeparator && hasValidSecondArgument && hasValidSecondCommaSeparator
            && hasValidThirdArgument && hasClosingBrace;

    if (!isValidPatternRes) {
        throw QuerySyntaxError("Syntax Error: Invalid pattern syntax");
    }

    return isValidPatternRes;
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


            // total of 5 more tokens: '(' 'stmtRef' ',' 'stmtRef' ')'
            if (nextPos + 4 >= tokens.size()) {
                throw QuerySyntaxError("Syntax Error occurred: " + tokenValue + " relationship has wrong format");
            }
            abstraction = tokenValue == "Follows" ? SimpleProgram::DesignAbstraction::FOLLOWS : SimpleProgram::DesignAbstraction::PARENT;
            bool isValidRs = isValidRelationship(nextPos, abstraction);

            if (isValidRs) {
                bool areArgumentsValid = isValidRelationshipArguments(nextPos + 1, nextPos + 3, true);
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

    } else if (tokenValue == "Follows*" || tokenValue == "Parent*") {
        SimpleProgram::DesignAbstraction abstraction;
        std::vector<PQL::Synonym> args;

        if (nextPos < tokens.size()) {
            // total of 5 more tokens: '(' 'stmtRef' ',' 'stmtRef' ')'
            if (nextPos + 4 >= tokens.size()) {
                throw QuerySyntaxError("Syntax Error occurred: " + tokenValue + " relationship has wrong format");
            }

            abstraction = tokenValue == "Follows*" ? SimpleProgram::DesignAbstraction::FOLLOWST : SimpleProgram::DesignAbstraction::PARENTT;
            bool isValidRs = isValidRelationship(nextPos, abstraction);

            if (isValidRs) {
                bool areArgumentsValid = isValidRelationshipArguments(nextPos + 1, nextPos + 3, true);
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
    } else if (tokenValue == "Modifies" || tokenValue == "Uses") {
        std::vector<PQL::Synonym> args;
        auto abstraction = tokenValue == "Modifies" ? SimpleProgram::DesignAbstraction::MODIFIESS : SimpleProgram::DesignAbstraction::USESS;
        if (nextPos < tokens.size()) {
            auto nextToken = tokens[nextPos];
            // total of 5 more tokens
            // '(' {stmtRef} ',' {entRef} ')'
            if (nextPos + 4 >= tokens.size()) {
                throw QuerySyntaxError("Syntax Error occurred: " + tokenValue + " relationship has wrong format");
            }

            bool isValidRs = isValidRelationship(nextPos, abstraction);

            if (isValidRs) {
                bool areArgumentsValid = isValidRelationshipArguments(nextPos + 1, nextPos + 3, false);
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

        }
    } else {
        throw QuerySyntaxError("Syntax Error: Invalid relationship");
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
    } else if (tokenValue == "_") {
        return SimpleProgram::DesignEntity::WILDCARD;
    } else {
        throw QuerySyntaxError("Syntax Error for the following token IN PARSER: " + tokenValue);
    }
}

SimpleProgram::DesignEntity QueryParser::getEntityTypeFromSynonym(const std::shared_ptr<QueryToken>& token) {
    auto declarationVerification = verifyDeclarationExists(token);
    auto hasDeclaration = std::get<0>(declarationVerification);

    if (hasDeclaration) {
        return std::get<1>(declarationVerification);
    }

    if (token->getType() == QPS::TokenType::INTEGER) {
        return SimpleProgram::DesignEntity::STMT_NO;
    }

    if (token->getType() == QPS::TokenType::CONSTANT_STRING) {
        return SimpleProgram::DesignEntity::IDENT;
    }

    if (token->getType() == QPS::TokenType::WILDCARD) {
        return SimpleProgram::DesignEntity::WILDCARD;
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
    int lastSemicolonPos = -1;
    int curr = pos;
    bool hasMultipleDeclarations = false;
    while (curr < tokens.size()) {
        auto currToken = tokens[curr];

        if (currToken->getValue() == "Select" && pos != (lastSemicolonPos + 1)) {
            pos = lastSemicolonPos + 1;
            break;
        }

        if (currToken->getValue() == ";") {
            if (!hasMultipleDeclarations && (curr - lastSemicolonPos) > 3) {
                throw QuerySyntaxError("Syntax Error: Invalid declaration syntax");
            }
            lastSemicolonPos = curr;
            // Add all the declarations in the array.
            // First element should always be the type, following elements will be the synonyms used
            auto entityType = getEntityType(currDeclarations[0]);
            for (int i = 1; i < currDeclarations.size(); i++) {
                auto currIdentity = currDeclarations[i]->getValue();
                bool hasDuplicateDeclarations = verifyNoDuplicateDeclarations(currIdentity);
                if (hasDuplicateDeclarations) {
                    throw QuerySemanticError("Semantic Error: A synonym name can only be declared once.");
                }
                auto declaration = PQL::Synonym(entityType, currIdentity);
                declarations.push_back(declaration);
            }
            hasMultipleDeclarations = false;
            currDeclarations.clear();
            curr++;
            continue;
        }

        if (currToken->getValue() == ",") {
            hasMultipleDeclarations = true;
            curr++;
            continue;
        }

        currDeclarations.push_back(currToken);
        curr++;
    }
    pos = lastSemicolonPos + 1;
    return declarations;
}

std::vector<PQL::Clause> QueryParser::parseClauses() {
    std::vector<PQL::Clause> clauses;

    while (pos < tokens.size()) {
        auto currToken = tokens[pos];
        std::string tokenValue = currToken->getValue();

        // only have two options, either 'such that' clause, or 'pattern' clause

        if (tokenValue == "pattern") {
            // Loop through to find closing brace index;
            int curr = pos+1;
            while (curr < tokens.size()) {
                auto token = tokens[curr];
                auto val = token->getValue();
                if (val == ")") {
                    break;
                }
                curr++;
            }
            // Will throw error and terminate within isValidPattern, hence always true;
            isValidPattern(pos+1,curr);

            std::vector<PQL::Synonym> patternArgs;
            auto patternArgType = getEntityTypeFromSynonym(tokens[pos+1]);
            PQL::Synonym patternArg = PQL::Synonym(patternArgType, tokens[pos+1]->getValue());
            auto arg1 = PQL::Synonym(getEntityTypeFromSynonym(tokens[pos+3]), tokens[pos+3]->getValue());
            patternArgs.push_back(patternArg);
            patternArgs.push_back(arg1);
            SimpleProgram::DesignAbstraction clauseType;

            if (patternArgType == SimpleProgram::DesignEntity::ASSIGN) {
                SimpleProgram::DesignEntity exprType;
                std::string exprId;
                if ((curr - (pos + 5)) > 1) {
                    std::string id;
                    for (int i = pos + 5; i < curr; i++) {
                        id += tokens[i]->getValue();
                    }
                    exprType = SimpleProgram::DesignEntity::PARTIAL_EXPR;
                    exprId = id;
                } else {
                    if (tokens[pos + 5]->getType() == QPS::TokenType::WILDCARD) {
                        exprType = SimpleProgram::DesignEntity::WILDCARD;
                    } else {
                        exprType = SimpleProgram::DesignEntity::EXPR;
                    }
                    exprId = tokens[pos + 5]->getValue();
                }

                auto arg2 = PQL::Synonym(exprType, exprId);
                patternArgs.push_back(arg2);
                clauseType = SimpleProgram::DesignAbstraction::PATTERN_ASSIGN;
            } else if (patternArgType == SimpleProgram::DesignEntity::IF) {
                auto arg2 = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
                auto arg3 = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
                patternArgs.push_back(arg2);
                patternArgs.push_back(arg3);
                clauseType = SimpleProgram::DesignAbstraction::PATTERN_IF;
            } else {
                auto arg2 = PQL::Synonym(SimpleProgram::DesignEntity::WILDCARD, "_");
                patternArgs.push_back(arg2);
                clauseType = SimpleProgram::DesignAbstraction::PATTERN_WHILE;
            }

            auto clause = PQL::Clause(clauseType, patternArgs);
            clauses.push_back(clause);
            pos = curr + 1;

        } else {
            if (pos + 1 < tokens.size()) {
                auto nextToken = tokens[++pos];
                auto nextTokenValue = nextToken->getValue();
                if (tokenValue != "such" && nextTokenValue != "that") {
                    pos++;
                    continue;
                }
                pos++;
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
        throw QuerySyntaxError("Syntax Error: Select clause should come first");
    }

    if (pos + 1 >= tokens.size()) {
        throw QuerySyntaxError("Syntax Error: Missing synonym for Select clause");
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
    pos += 1;
    return selectSynonym;
}