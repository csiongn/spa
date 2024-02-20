#pragma once

#include "SP/Tokenizer.h"
#include "SP/Parser.h"
#include "SP/DesignExtractor.h"
#include "Models/SimpleProgram.h"

#include <iostream>

class SPFacade {
public:
    SPFacade(std::shared_ptr<PKBFacade> pkbFacade, const std::string& input)
          : designExtractor(pkbFacade),
          tokenizer(Tokenizer(input)), pkbFacade(pkbFacade)
    {}

    void populatePKB();


    void insertEntity(SimpleProgram::DesignEntity entity, const std::unordered_set<int>& value);
    void insertEntity(SimpleProgram::DesignEntity entity, const std::unordered_set<std::string>& value);

    void insertRelationship(SimpleProgram::DesignAbstraction relationship, const std::unordered_map<int, std::unordered_set<int>>& map);

    void insertRelationship(SimpleProgram::DesignAbstraction relationship, int key, const std::unordered_set<int>& value);

    void insertRelationship(SimpleProgram::DesignAbstraction relationship, const std::unordered_map<int, std::unordered_set<std::string>>& map);
    void insertRelationship(SimpleProgram::DesignAbstraction relationship, int key, const std::unordered_set<std::string>& value);

private:
    DesignExtractor designExtractor;
    Tokenizer tokenizer;
protected:
    std::shared_ptr<PKBFacade> pkbFacade;
};