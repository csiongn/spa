#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "PKB.h"
#include "SP/TNode.h"


PKB::PKB() {
    // Initialize PKB with Database and PKBFacade
    auto db = std::make_shared<Database>();
    pkbFacade = std::make_shared<PKBFacade>(db);
}
