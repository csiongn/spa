#include <iostream>
#include <string>
#include <vector>

#include "QueryTokenizer.h"
#include "QueryToken.h"

std::vector<std::shared_ptr<QueryToken>> QueryTokenizer::tokenize(std::string &query) {
    // TODO: Implement tokenize()

    std::shared_ptr<QueryToken> token = std::make_shared<QueryToken>();
    std::vector<std::shared_ptr<QueryToken>> tokenized = std::vector(1, token);
    return tokenized;
}