#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "QPS/QueryToken.h"
class ClauseValidator {
 protected:
  bool hasSemanticError;
  std::vector<PQL::Synonym> declarations;

 private:
  bool isValidIdent(std::shared_ptr<QueryToken> token);

  bool isStmtRef(std::shared_ptr<QueryToken> token);

  bool isEntRef(std::shared_ptr<QueryToken> token);

  bool isStmtOrEntRef(std::shared_ptr<QueryToken> token);

  bool isElement(std::shared_ptr<QueryToken> token);

  bool isExpression(std::shared_ptr<QueryToken> token);

  bool isExpressionSpec(std::vector<std::shared_ptr<QueryToken>> &tokens);

  bool isAttrCompare(std::vector<std::shared_ptr<QueryToken>> attrTokens);

  void validateAttrRef(std::shared_ptr<QueryToken> attrToken);

 public:
  explicit ClauseValidator(std::vector<PQL::Synonym> &declarations);

  void setSemanticError();

  bool hasError();

  bool isSynonym(std::shared_ptr<QueryToken> token);

  bool isPartialExpression(std::shared_ptr<QueryToken> token);

  void validateStmtRef(std::shared_ptr<QueryToken> token);

  void validateEntRef(std::shared_ptr<QueryToken> token);

  void validateStmtOrEntRef(std::shared_ptr<QueryToken> token);

  void validateAttrCompare(std::vector<std::shared_ptr<QueryToken>> attrTokens);

  void validateDeclarationExists(std::shared_ptr<QueryToken> synonymToken);

  void validateDeclarationExists(std::string synonymIdentity);

  void validateSelectClauseToken(std::shared_ptr<QueryToken> selectToken);

  static void validateHasSelectSynonym(
	  std::vector<std::shared_ptr<QueryToken>> &clauseTokens);

  void validatePatternSynonym(std::shared_ptr<QueryToken> &patternToken);

  void validatePatternAssign(
	  std::vector<std::shared_ptr<QueryToken>> &patternArgs);

  void validatePatternIf(
	  std::vector<std::shared_ptr<QueryToken>> &patternArgs);

  void validatePatternWhile(
	  std::vector<std::shared_ptr<QueryToken>> &patternArgs);

  void validatePatternArgs(
	  SimpleProgram::DesignAbstraction patternType,
	  std::vector<std::shared_ptr<QueryToken>> &patternArgs);

  void validateUsesModifiesArgs(
	  std::vector<std::shared_ptr<QueryToken>> &suchThatArgs);

  void validateFollowsParentsArgs(
	  std::vector<std::shared_ptr<QueryToken>> &suchThatArgs);

  void validateCallsArgs(
	  std::vector<std::shared_ptr<QueryToken>> &suchThatArgs);

  void validateWith(std::vector<std::shared_ptr<QueryToken>> &withArgs);
};