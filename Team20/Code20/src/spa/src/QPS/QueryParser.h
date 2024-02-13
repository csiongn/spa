#pragma once

#include <iostream>

#include "QueryToken.h"
#include "Models/PQL.h"

class QueryParser {
protected:
    std::vector<std::shared_ptr<QueryToken>> tokens;
    int pos;
public:
    QueryParser(std::vector<std::shared_ptr<QueryToken>> tokens);

    PQL::Query parse();
private:
    static SimpleProgram::DesignEntity getEntityType(const std::shared_ptr<QueryToken>& token);
    static PQL::Synonym createSynonym(std::shared_ptr<QueryToken>& token);
    static std::tuple<bool, SimpleProgram::DesignEntity> verifyDeclarationExists(const std::string& synonym, const std::vector<PQL::Synonym>& declarations);
    bool isValidRelationship(int start);
    bool isValidRelationshipArguments(int pos1, int pos2, const std::vector<PQL::Synonym>& declarations);
    bool isValidAssignSynonym(std::shared_ptr<QueryToken>& token, const std::vector<PQL::Synonym>& declarations);
    bool isValidPattern(int start, const std::vector<PQL::Synonym>& declarations);

    static bool isStmtRef(std::shared_ptr<QueryToken>& token);
    static bool isEntRef(std::shared_ptr<QueryToken>& token);
    static bool isExpSpec(std::shared_ptr<QueryToken>& token);

    std::tuple<bool, SimpleProgram::DesignAbstraction, std::vector<PQL::Synonym>> getRelationshipAttributes(const std::vector<PQL::Synonym>& declarations);
    std::vector<PQL::Synonym> parseDeclarations();
    std::vector<PQL::Clause> parseClauses(const std::vector<PQL::Synonym>& declarations);
    PQL::Synonym parseSelectClause(const std::vector<PQL::Synonym>& declarations);
    void removeUnusedDeclarations(std::shared_ptr<std::vector<PQL::Synonym>>);
};