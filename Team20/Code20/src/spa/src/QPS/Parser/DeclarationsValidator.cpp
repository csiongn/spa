#include "DeclarationsValidator.h"
#include "QPS/QuerySyntaxError.h"
#include <unordered_set>

void DeclarationsValidator::validateSynonymType(std::shared_ptr<QueryToken>& token) {
    auto tokenValue = token->getValue();
    std::unordered_set<std::string> validTypes( {"procedure",
                                                 "stmt",
                                                 "read",
                                                 "print",
                                                 "assign",
                                                 "call",
                                                 "while",
                                                 "if",
                                                 "variable",
                                                 "constant"} );

    bool isValidSynonymType = validTypes.find(tokenValue) != validTypes.end();

    if (!isValidSynonymType) {
        throw QuerySyntaxError("Syntax Error: Invalid declaration synonym type");
    }
}

void DeclarationsValidator::validateNoDuplicates(std::vector<PQL::Synonym>& declarations, std::shared_ptr<QueryToken>& token) {
    for (auto& declaration : declarations) {
        if (declaration.identity == token->getValue()) {
            throw QuerySyntaxError("Syntax Error: Duplicate declaration synonym");
        }
    }
}