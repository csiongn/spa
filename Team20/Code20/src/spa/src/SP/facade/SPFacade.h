#pragma once

#include "SP/Tokenizer.h"
#include "SP/Parser.h"
#include "SP/DesignExtractor.h"
#include <iostream>

class SPFacade {
public:
    SPFacade(std::shared_ptr<PKBFacade> pkbFacade, const std::string& input)
          : designExtractor(pkbFacade),
          tokenizer(Tokenizer(input))
    {}

    void populatePKB();

private:
    DesignExtractor designExtractor;
    Tokenizer tokenizer;
};