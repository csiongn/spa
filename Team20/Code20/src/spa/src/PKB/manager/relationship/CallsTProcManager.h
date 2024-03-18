#pragma once

#include <string>

#include "RelationshipManager.h"

// proc String, proc string
class CallsTProcManager : public RelationshipManager<std::string, std::string> {
};