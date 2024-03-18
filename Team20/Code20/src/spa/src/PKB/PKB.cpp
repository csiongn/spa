#include <memory>

#include "PKB.h"


PKB::PKB() {
    // Initialize PKB with Database and PKBFacade
    auto db = std::make_shared<Database>();
    pkbFacade = std::make_shared<PKBFacade>(db);
}
