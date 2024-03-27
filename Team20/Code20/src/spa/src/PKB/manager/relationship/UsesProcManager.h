#pragma once

#include <string>

#include "RelationshipManager.h"

// proc String, string
class UsesProcManager : public RelationshipManager<std::string, std::string> {
};
