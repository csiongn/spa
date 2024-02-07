#pragma once
#include <iostream>
#include "QueryToken.h"
class QueryTokenizer {
public:
    std::vector<std::shared_ptr<QueryToken>> tokenize(std::string &query);
};