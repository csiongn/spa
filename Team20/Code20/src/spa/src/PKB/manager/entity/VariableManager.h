#pragma once

#include "EntityManager.h"

#include <string>

// entRef
// store a list of all variables represented as strings in this table
class VariableManager : public EntityManager<std::string> {
  // store as individual variables from SP, not partial string etc.
};