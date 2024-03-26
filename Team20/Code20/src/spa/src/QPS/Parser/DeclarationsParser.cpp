#include "DeclarationsParser.h"
#include "QPS/QuerySyntaxError.h"
#include "QPS/Utils/ParseUtils.h"

#include <utility>

DeclarationsParser::DeclarationsParser(std::vector<std::shared_ptr<QueryToken>> declarationTokens, std::vector<int> semicolonPos, std::shared_ptr<DeclarationsValidator>& validator)
    : declarationTokens(std::move(declarationTokens)), semicolonPos(std::move(semicolonPos)), validator(validator) {}

std::vector<PQL::Synonym> DeclarationsParser::parseDeclarations() {
    std::vector<PQL::Synonym> declarations;
    int count = 0;
    int lastSynPos = 0;
    for (auto pos : semicolonPos) {
        SimpleProgram::DesignEntity synonymType;

        while (count != pos) {
            auto currToken = declarationTokens[count];
            if (count == lastSynPos) {
                DeclarationsValidator::validateSynonymType(currToken);
                synonymType = QueryEvaluator::ParseUtils::getSynonymType(currToken);
                if (synonymType == SimpleProgram::DesignEntity{}) {
                    throw QuerySyntaxError("Syntax Error: Invalid synonym type");
                }
                count++;
                continue;
            }

            if (currToken->getValue() == ",") {
                count++;
                continue;
            }
            validator->validateSynonymName(currToken);
            PQL::Synonym synonym = QueryEvaluator::ParseUtils::createSynonym(synonymType, currToken);
            validator->validateNoDuplicates(declarations, currToken);
            declarations.push_back(synonym);
            count++;
        }
        lastSynPos = pos + 1;
        count++;
    }

    return declarations;
}