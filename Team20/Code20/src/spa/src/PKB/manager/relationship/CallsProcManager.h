#pragma once

#include <string>

#include "RelationshipManager.h"

// proc String, proc string
class CallsProcManager : public RelationshipManager<std::string, std::string> {
};