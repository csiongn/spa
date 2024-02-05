#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

// Need to initialize Database and PKBFacade to interact with Database from SP and QPS

#include "PKB/Database.h"
#include "PKB/facade/PKBFacade.h"

class PKB {

public:
    PKB();

    std::shared_ptr<PKBFacade> pkbFacade;
};