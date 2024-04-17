#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include "../lib/catch.hpp"

#include "Models/SimpleProgram.h"
#include "PKB/PKB.h"
#include "SP/SPController.h"
#include "../TestUtils.h"

TEST_CASE("SPController Test") {
  // initialise SPController
  auto pkb = std::make_shared<PKB>();
  auto pkbFacade = pkb->pkbFacade;
  // no input for SPController
  auto spController = SPController(pkbFacade, "");
  std::unordered_map<int, std::unordered_set<int>> follows = {
	  {1, {2}},
	  {2, {3}},
  };

  std::unordered_map<int, std::unordered_set<int>> followsT = {
	  {1, {2, 3, 4, 5, 6, 7}},
	  {2, {3, 4, 5, 6, 7, 8}},
  };

  std::unordered_map<int, std::unordered_set<int>> parent = {
	  {3, {4}},
	  {4, {5}},
  };

  std::unordered_map<int, std::unordered_set<int>> parentT = {
	  {3, {4, 5, 6, 7, 8, 9}},
	  {4, {5, 6, 7, 8, 9, 10}},
  };

  // Entities
  std::unordered_set<int> stmtSet = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::unordered_set<std::string> varSet = {"x", "y", "z"};

  SECTION("Test insertion of pattern") {
	// TODO: Add test cases for pattern
  }
}
