#pragma once

#include <string>

#include "RelationshipManager.h"

// To support attributes
// variable, stmt#
class PrintVarManager : public RelationshipManager<std::string, int> {
};