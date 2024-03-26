#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <tuple>

#include "QPS/QueryToken.h"
#include "Models/PQL.h"
#include "QPS/QueryError.h"

class QueryParser {
 protected:
  std::vector<std::shared_ptr<QueryToken>> tokens;
  std::vector<PQL::Synonym> initialDeclarations;
  std::vector<PQL::Synonym> usedDeclarations;
  std::vector<PQL::Clause> clauses;
  std::vector<std::shared_ptr<QueryError>> errors;
  int pos;

 public:
  explicit QueryParser(std::vector<std::shared_ptr<QueryToken>> tokens);
  PQL::Query parse();

 private:
  static SimpleProgram::DesignEntity getEntityType(const std::shared_ptr<QueryToken> &token);
  SimpleProgram::DesignEntity getEntityTypeFromSynonym(const std::shared_ptr<QueryToken> &token);
  PQL::Synonym createSynonym(std::shared_ptr<QueryToken> &token);
  std::tuple<bool, SimpleProgram::DesignEntity> verifyDeclarationExists(const std::shared_ptr<QueryToken> &token);
  bool verifyNoDuplicateDeclarations(std::string synonymIdentity);
  bool isValidRelationship(int start, SimpleProgram::DesignAbstraction relationship);
  bool isValidRelationshipArguments(int pos1, int pos2, bool isFollowsOrParent);
  bool isValidPatternSynonym(std::shared_ptr<QueryToken> &token);
  bool isValidPattern(int start, int end);
  bool isStmtSubtype(std::shared_ptr<QueryToken> &token);

  static bool isLetter(char c);
  static bool isDigit(char c);
  static bool isName(const std::string &str);
  static bool isInteger(const std::string &str);
  bool isFactor(const std::shared_ptr<QueryToken> &token);
  bool isExpSpec(const std::vector<std::shared_ptr<QueryToken>> &expSpecTokens);

  static bool isStmtRef(std::shared_ptr<QueryToken> &token);
  static bool isEntRef(std::shared_ptr<QueryToken> &token);

  std::tuple<bool, SimpleProgram::DesignAbstraction, std::vector<PQL::Synonym>> getRelationshipAttributes();
  std::vector<PQL::Synonym> parseDeclarations();
  std::vector<PQL::Clause> parseClauses();
  PQL::Synonym parseSelectClause();
  std::vector<int> parseSemicolons();
  std::vector<std::shared_ptr<QueryToken>> splitTokens(int start, int end);
  bool validateSynonymType(std::shared_ptr<QueryToken> &token);
  void checkErrors();
  void addError(QueryError& err);
};