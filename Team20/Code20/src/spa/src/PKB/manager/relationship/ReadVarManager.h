#pragma once

#include <string>

#include "RelationshipManager.h"

// To support attributes
// variable, stmt#
class ReadVarManager : public RelationshipManager<std::string, int> {
};