#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "Models/SimpleProgram.h"
#include "SP/SPController.h"

class SPFacade {
 public:
  SPFacade(std::shared_ptr<IPKBWriter> pkbWriter, const std::string &input)
	  : spController(pkbWriter, input) {}

  void populatePKB();

 private:
  SPController spController;
};