#include <chrono>
#include <vector>
#include <string>
#include <memory>

#include "../lib/catch.hpp"

#include "../../unit_testing/src/TestUtils.h"
#include "SPA.h"

constexpr size_t TIMEOUT = 3500;

TEST_CASE("Integration test SP to PKB") {
  std::string testProgram = "procedure main {"
    "x = v * y + z * t;"
    "if ((x > 2) || ((y == 1) && (!(x == 0)))) then {"
    "x = v * y + z * t;"
    "y = 1 + 3 / 5 % x;"
    "if (x>2) then {"
    "x = 1;"
    "} else {"
    "a = (x % y) + ((y / 1) * 3) + a;"
    "}"
    "} else {"
    "a = 5;"
    "}"
    "while (x < 2) {"
    "x = 1;"
    "}"
    "}"
    "procedure avg {"
    "avg = num1 + num2;"
    "avg = avg / 2;"
    "print avg;"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    SPFacade.populatePKB();
    THEN("The PKB should store the correct entities and variables") {
      // Check procedures
      std::vector<std::string> expectedProcs = {"main", "avg"};
      std::vector<std::string> actualProcs = pkbFacade->getAllProcedures();
      REQUIRE(checkVecValuesEqual(expectedProcs, actualProcs));

      // Check variables
      std::vector<std::string> expectedValues = {"a", "t", "v", "x", "y", "z", "avg", "num1", "num2"};
      std::vector<std::string> actualValues = pkbFacade->getAllVariables();
      REQUIRE(checkVecValuesEqual(expectedValues, actualValues));

      // Check literals
      std::vector<int> expectedliterals = {0, 1, 2, 3, 5};
      std::vector<int> actualLiterals = pkbFacade->getAllConstants();
      REQUIRE(checkVecValuesEqual(expectedliterals, actualLiterals));

      // Check statements
      std::vector<int> expectedStmts = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
      std::vector<int> actualStmts = pkbFacade->getAllStatementNum();
      REQUIRE(checkVecValuesEqual(expectedStmts, actualStmts));

      // Check if statements
      std::vector<int> expectedIfStmts = {2, 5};
      std::vector<int> actualIfStmts = pkbFacade->getAllIfStmtNum();
      REQUIRE(checkVecValuesEqual(expectedIfStmts, actualIfStmts));

      // Check print statements
      std::vector<int> expectedPrintStmts = {13};
      std::vector<int> actualPrintStmts = pkbFacade->getAllPrintStmtNum();
      REQUIRE(checkVecValuesEqual(expectedPrintStmts, actualPrintStmts));

      // Check assignment statements
      std::vector<int> expectedAssignStmts = {1, 3, 4, 6, 7, 8, 10, 11, 12};
      std::vector<int> actualAssignStmts = pkbFacade->getAllAssignStmtNum();
      REQUIRE(checkVecValuesEqual(expectedAssignStmts, actualAssignStmts));

      // No read statements
      REQUIRE(pkbFacade->getAllReadStmtNum().empty());

      // No while statements
      std::vector<int> expectedWhileStmts = {9};
      std::vector<int> actualWhileStmts = pkbFacade->getAllWhileStmtNum();
      REQUIRE(checkVecValuesEqual(expectedWhileStmts, actualWhileStmts));
    }

    AND_THEN("The PKB should store the correct relationships") {
      // Check Next relationships
      REQUIRE(pkbFacade->containsNextRelationship(1, 2));
      REQUIRE(pkbFacade->containsNextRelationship(2, 3));
      REQUIRE(pkbFacade->containsNextRelationship(2, 8));
      REQUIRE(pkbFacade->containsNextRelationship(3, 4));
      REQUIRE(pkbFacade->containsNextRelationship(4, 5));
      REQUIRE(pkbFacade->containsNextRelationship(5, 6));
      REQUIRE(pkbFacade->containsNextRelationship(5, 7));
      REQUIRE(pkbFacade->containsNextRelationship(6, 9));
      REQUIRE(pkbFacade->containsNextRelationship(7, 9));
      REQUIRE(pkbFacade->containsNextRelationship(8, 9));
      REQUIRE(pkbFacade->containsNextRelationship(9, 10));
      REQUIRE(pkbFacade->containsNextRelationship(10, 9));
      REQUIRE(pkbFacade->containsNextRelationship(11, 12));
      REQUIRE(pkbFacade->containsNextRelationship(12, 13));

      // Check correct NextT relationships
      REQUIRE(pkbFacade->hasNextTRelationship());

      REQUIRE(pkbFacade->containsNextTRelationship(1, 4));
      REQUIRE(pkbFacade->containsNextTRelationship(1, 6));
      REQUIRE(pkbFacade->containsNextTRelationship(1, 7));
      REQUIRE_FALSE(pkbFacade->containsNextTRelationship(6, 7));
      REQUIRE_FALSE(pkbFacade->containsNextTRelationship(1, 11));

      // Check no wrong false recursive NextT relationships
      REQUIRE_FALSE(pkbFacade->containsNextTRelationship(1, 1));
      REQUIRE_FALSE(pkbFacade->containsNextTRelationship(2, 2));
      REQUIRE_FALSE(pkbFacade->containsNextTRelationship(3, 3));
      REQUIRE_FALSE(pkbFacade->containsNextTRelationship(4, 4));
      REQUIRE_FALSE(pkbFacade->containsNextTRelationship(5, 5));
      REQUIRE_FALSE(pkbFacade->containsNextTRelationship(6, 6));


      // Statements that have a NextT relationship from
      const std::vector<int>& expectedContainsNextTFrom{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
      const auto& actualContainsNextTFrom = pkbFacade->getNextT();
      REQUIRE(checkVecValuesEqual(expectedContainsNextTFrom, actualContainsNextTFrom));

      REQUIRE(pkbFacade->containsNextT(1));
      REQUIRE(pkbFacade->containsNextT(2));
      REQUIRE(pkbFacade->containsNextT(3));
      REQUIRE(pkbFacade->containsNextT(4));
      REQUIRE(pkbFacade->containsNextT(5));
      REQUIRE(pkbFacade->containsNextT(6));
      REQUIRE(pkbFacade->containsNextT(7));
      REQUIRE(pkbFacade->containsNextT(8));
      REQUIRE(pkbFacade->containsNextT(9));
      REQUIRE(pkbFacade->containsNextT(10));
      REQUIRE(pkbFacade->containsNextT(11));
      REQUIRE(pkbFacade->containsNextT(12));
      REQUIRE_FALSE(pkbFacade->containsNextT(13));
      REQUIRE_FALSE(pkbFacade->containsNextT(14));

      // Statements that have a NextT relationship to
      const std::vector<int>& expectedContainsNextTTo{2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13};
      const auto& actualContainsNextTTo = pkbFacade->getNextTReverse();
      REQUIRE(checkVecValuesEqual(expectedContainsNextTTo, actualContainsNextTTo));

      REQUIRE_FALSE(pkbFacade->containsNextTReverse(1));
      REQUIRE(pkbFacade->containsNextTReverse(2));
      REQUIRE(pkbFacade->containsNextTReverse(3));
      REQUIRE(pkbFacade->containsNextTReverse(4));
      REQUIRE(pkbFacade->containsNextTReverse(5));
      REQUIRE(pkbFacade->containsNextTReverse(6));
      REQUIRE(pkbFacade->containsNextTReverse(7));
      REQUIRE(pkbFacade->containsNextTReverse(8));
      REQUIRE(pkbFacade->containsNextTReverse(9));
      REQUIRE(pkbFacade->containsNextTReverse(10));
      REQUIRE_FALSE(pkbFacade->containsNextTReverse(11));
      REQUIRE(pkbFacade->containsNextTReverse(12));
      REQUIRE(pkbFacade->containsNextTReverse(13));
      REQUIRE_FALSE(pkbFacade->containsNextTReverse(14));

      std::cout << "========== TESTING NextT RUN TIME COMPUTATION ==========" << std::endl;
      const std::vector<int>& expectNextTFrom1 = {2, 3, 4, 5, 6, 7, 8, 9, 10};
      auto start1_1 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom1 = pkbFacade->getNextT(1);
      auto end1_1 = std::chrono::high_resolution_clock::now();
      auto duration1_1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_1 - start1_1).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom1, actualNextTFrom1));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(1): " <<  duration1_1 << " ms" << std::endl;
      REQUIRE(duration1_1 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom2 = {3, 4, 5, 6, 7, 8, 9, 10};
      auto start1_2 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom2 = pkbFacade->getNextT(2);
      auto end1_2 = std::chrono::high_resolution_clock::now();
      auto duration1_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_2 - start1_2).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom2, actualNextTFrom2));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(2): " <<  duration1_2 << " ms" << std::endl;
      REQUIRE(duration1_2 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom3 = {4, 5, 6, 7, 9, 10};
      auto start1_3 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom3 = pkbFacade->getNextT(3);
      auto end1_3 = std::chrono::high_resolution_clock::now();
      auto duration1_3 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_3 - start1_3).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom3, actualNextTFrom3));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(3): " <<  duration1_3 << " ms" << std::endl;
      REQUIRE(duration1_3 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom4 = {5, 6, 7, 9, 10};
      auto start1_4 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom4 = pkbFacade->getNextT(4);
      auto end1_4 = std::chrono::high_resolution_clock::now();
      auto duration1_4 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_4 - start1_4).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom4, actualNextTFrom4));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(4): " <<  duration1_4 << " ms" << std::endl;
      REQUIRE(duration1_4 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom5 = {6, 7, 9, 10};
      auto start1_5 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom5 = pkbFacade->getNextT(5);
      auto end1_5 = std::chrono::high_resolution_clock::now();
      auto duration1_5 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_5 - start1_5).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom5, actualNextTFrom5));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(5): " <<  duration1_5 << " ms" << std::endl;
      REQUIRE(duration1_5 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom6 = {9, 10};
      auto start1_6 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom6 = pkbFacade->getNextT(6);
      auto end1_6 = std::chrono::high_resolution_clock::now();
      auto duration1_6 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_6 - start1_6).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom6, actualNextTFrom6));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(6): " <<  duration1_6 << " ms" << std::endl;
      REQUIRE(duration1_6 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom7 = {9, 10};
      auto start1_7 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom7 = pkbFacade->getNextT(7);
      auto end1_7 = std::chrono::high_resolution_clock::now();
      auto duration1_7 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_7 - start1_7).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom7, actualNextTFrom7));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(7): " <<  duration1_7 << " ms" << std::endl;
      REQUIRE(duration1_7 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom8 = {9, 10};
      auto start1_8 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom8 = pkbFacade->getNextT(8);
      auto end1_8 = std::chrono::high_resolution_clock::now();
      auto duration1_8 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_8 - start1_8).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom8, actualNextTFrom8));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(8): " <<  duration1_8 << " ms" << std::endl;
      REQUIRE(duration1_8 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom9 = {9, 10};
      auto start1_9 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom9 = pkbFacade->getNextT(9);
      auto end1_9 = std::chrono::high_resolution_clock::now();
      auto duration1_9 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_9 - start1_9).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom9, actualNextTFrom9));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(9): " <<  duration1_9 << " ms" << std::endl;
      REQUIRE(duration1_9 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom10 = {9, 10};
      auto start1_10 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom10 = pkbFacade->getNextT(10);
      auto end1_10 = std::chrono::high_resolution_clock::now();
      auto duration1_10 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_10 - start1_10).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom10, actualNextTFrom10));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(10): " <<  duration1_10 << " ms" << std::endl;
      REQUIRE(duration1_10 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom11 = {12, 13};
      auto start1_11 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom11 = pkbFacade->getNextT(11);
      auto end1_11 = std::chrono::high_resolution_clock::now();
      auto duration1_11 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_11 - start1_11).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom11, actualNextTFrom11));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(11): " <<  duration1_11 << " ms" << std::endl;
      REQUIRE(duration1_11 <= TIMEOUT);

      const std::vector<int>& expectNextTFrom12 = {13};
      auto start1_12 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTFrom12 = pkbFacade->getNextT(12);
      auto end1_12 = std::chrono::high_resolution_clock::now();
      auto duration1_12 = std::chrono::duration_cast<std::chrono::milliseconds>(end1_12 - start1_12).count();

      REQUIRE(checkVecValuesEqual(expectNextTFrom12, actualNextTFrom12));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextT(12): " <<  duration1_12 << " ms" << std::endl;
      REQUIRE(duration1_12 <= TIMEOUT);

      const std::vector<int>& expectNextTTo2 = {1};
      auto start2_1 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTTo2 = pkbFacade->getNextTReverse(2);
      auto end2_1 = std::chrono::high_resolution_clock::now();
      auto duration2_1 = std::chrono::duration_cast<std::chrono::milliseconds>(end2_1 - start2_1).count();

      REQUIRE(checkVecValuesEqual(expectNextTTo2, actualNextTTo2));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextTReverse(2): " <<  duration2_1 << " ms" << std::endl;
      REQUIRE(duration2_1 <= TIMEOUT);

      const std::vector<int>& expectNextTTo3 = {1, 2};
      auto start2_2 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTTo3 = pkbFacade->getNextTReverse(3);
      auto end2_2 = std::chrono::high_resolution_clock::now();
      auto duration2_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2_2 - start2_2).count();

      REQUIRE(checkVecValuesEqual(expectNextTTo3, actualNextTTo3));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextTReverse(3): " <<  duration2_2 << " ms" << std::endl;
      REQUIRE(duration2_2 <= TIMEOUT);

      const std::vector<int>& expectNextTTo4 = {1, 2, 3};
      auto start2_3 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTTo4 = pkbFacade->getNextTReverse(4);
      auto end2_3 = std::chrono::high_resolution_clock::now();
      auto duration2_3 = std::chrono::duration_cast<std::chrono::milliseconds>(end2_3 - start2_3).count();

      REQUIRE(checkVecValuesEqual(expectNextTTo4, actualNextTTo4));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextTReverse(4): " <<  duration2_3 << " ms" << std::endl;
      REQUIRE(duration2_3 <= TIMEOUT);

      const std::vector<int>& expectNextTTo5 = {1, 2, 3, 4};
      auto start2_4 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTTo5 = pkbFacade->getNextTReverse(5);
      auto end2_4 = std::chrono::high_resolution_clock::now();
      auto duration2_4 = std::chrono::duration_cast<std::chrono::milliseconds>(end2_4 - start2_4).count();

      REQUIRE(checkVecValuesEqual(expectNextTTo5, actualNextTTo5));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextTReverse(5): " <<  duration2_4 << " ms" << std::endl;
      REQUIRE(duration2_4 <= TIMEOUT);

      const std::vector<int>& expectNextTTo6 = {1, 2, 3, 4, 5};
      auto start2_5 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTTo6 = pkbFacade->getNextTReverse(6);
      auto end2_5 = std::chrono::high_resolution_clock::now();
      auto duration2_5 = std::chrono::duration_cast<std::chrono::milliseconds>(end2_5 - start2_5).count();

      REQUIRE(checkVecValuesEqual(expectNextTTo6, actualNextTTo6));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextTReverse(6): " <<  duration2_5 << " ms" << std::endl;
      REQUIRE(duration2_5 <= TIMEOUT);

      const std::vector<int>& expectNextTTo7 = {1, 2, 3, 4, 5};
      auto start2_6 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTTo7 = pkbFacade->getNextTReverse(7);
      auto end2_6 = std::chrono::high_resolution_clock::now();
      auto duration2_6 = std::chrono::duration_cast<std::chrono::milliseconds>(end2_6 - start2_6).count();

      REQUIRE(checkVecValuesEqual(expectNextTTo7, actualNextTTo7));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextTReverse(7): " <<  duration2_6 << " ms" << std::endl;
      REQUIRE(duration2_6 <= TIMEOUT);

      const std::vector<int>& expectNextTTo8 = {1, 2};
      auto start2_7 = std::chrono::high_resolution_clock::now();
      const auto& actualNextTTo8 = pkbFacade->getNextTReverse(8);
      auto end2_7 = std::chrono::high_resolution_clock::now();
      auto duration2_7 = std::chrono::duration_cast<std::chrono::milliseconds>(end2_7 - start2_7).count();

      REQUIRE(checkVecValuesEqual(expectNextTTo8, actualNextTTo8));
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextTReverse(7): " <<  duration2_7 << " ms" << std::endl;
      REQUIRE(duration2_7 <= TIMEOUT);

      // All NextT Relationships
      const std::vector<std::pair<int, int>>& expectedNextTRelationships = {

        // Procedure "main"
        // Statement 1
        std::pair{1, 2}, std::pair{1, 3}, std::pair{1, 4}, std::pair{1, 5}, std::pair{1, 6},
        std::pair{1, 7}, std::pair{1, 8}, std::pair{1, 9}, std::pair{1, 10},

        // Statement 2
        std::pair{2, 3}, std::pair{2, 4}, std::pair{2, 5}, std::pair{2, 6}, std::pair{2, 7},
        std::pair{2, 8}, std::pair{2, 9}, std::pair{2, 10},

        // Statement 3
        std::pair{3, 4}, std::pair{3, 5}, std::pair{3, 6}, std::pair{3, 7}, std::pair{3, 9},
        std::pair{3, 10},

        // Statement 4
        std::pair{4, 5}, std::pair{4, 6}, std::pair{4, 7}, std::pair{4, 9}, std::pair{4, 10},

        // Statement 5
        std::pair{5, 6}, std::pair{5, 7}, std::pair{5, 9}, std::pair{5, 10},

        // Statement 6
        std::pair{6, 9}, std::pair{6, 10},

        // Statement 7
        std::pair{7, 9}, std::pair{7, 10},

        // Statement 8
        std::pair{8, 9}, std::pair{8, 10},

        // Statement 9
        std::pair{9, 9}, std::pair{9, 10},

        // Statement 10
        std::pair{10, 9}, std::pair{10, 10},


        // Procedure "avg"

        // Statement 11
        std::pair{11, 12}, std::pair{11, 13},

        // Statement 12
        std::pair{12, 13},

      };
      auto start = std::chrono::high_resolution_clock::now();
      const auto& actualNextTRelationships = pkbFacade->getNextTRelationships();
      auto end = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

      REQUIRE(expectedNextTRelationships == actualNextTRelationships);
      // Check that query time doesn't exceed 5 seconds
      std::cout << "Time taken to compute getNextTRelationships(): " <<  duration << " ms" << std::endl;
      REQUIRE(duration <= TIMEOUT);
      std::cout << "========================================================" << std::endl;

      REQUIRE(pkbFacade->hasAffectsRelationship());

      REQUIRE(pkbFacade->containsAffectsRelationship(3, 4));
      REQUIRE(pkbFacade->containsAffectsRelationship(3, 7));
      REQUIRE(pkbFacade->containsAffectsRelationship(4, 7));
      REQUIRE(pkbFacade->containsAffectsRelationship(11, 12));
      REQUIRE_FALSE(pkbFacade->containsAffectsRelationship(7, 7));
      REQUIRE_FALSE(pkbFacade->containsAffectsRelationship(3, 3));
      REQUIRE_FALSE(pkbFacade->containsAffectsRelationship(7, 3));
      REQUIRE_FALSE(pkbFacade->containsAffectsRelationship(12, 12));

      REQUIRE(pkbFacade->containsAffects(4));
      REQUIRE(pkbFacade->containsAffectsReverse(7));
      REQUIRE_FALSE(pkbFacade->containsAffectsReverse(11));
      REQUIRE(pkbFacade->containsAffectsReverse(12));

      const std::vector<int>& expectedAffectsFrom3 = {4, 7};
      auto startAffectsFrom3 = std::chrono::high_resolution_clock::now();
      const auto& actualAffectsFrom3 = pkbFacade->getAffects(3);
      auto endAffectsFrom3 = std::chrono::high_resolution_clock::now();
      auto durationAffectsFrom3 = std::chrono::duration_cast<std::chrono::milliseconds>(endAffectsFrom3 - startAffectsFrom3).count();

      REQUIRE(durationAffectsFrom3 <= TIMEOUT);
      REQUIRE(checkVecValuesEqual(expectedAffectsFrom3, actualAffectsFrom3));

      // All Affects relationships
      const std::vector<std::pair<int, int>>& expectedAffectsRelationships = {
        // Statement 3
        std::pair{3, 4}, std::pair{3, 7},

        // Statement 4
        std::pair{4, 7},

        // Statement 11
        std::pair{11, 12},
      };

      auto startAffectsAll = std::chrono::high_resolution_clock::now();
      const auto& actualAffectsRelationships = pkbFacade->getAffectsRelationships();
      auto endAffectsAll = std::chrono::high_resolution_clock::now();
      auto durationAffectsAll = std::chrono::duration_cast<std::chrono::milliseconds>(endAffectsAll - startAffectsAll).count();

      REQUIRE(checkVecValuesEqual(expectedAffectsRelationships, actualAffectsRelationships));
      std::cout << "Time taken to compute getAffectsRelationships(): " <<  durationAffectsAll << " ms" << std::endl;
      REQUIRE(durationAffectsAll <= TIMEOUT);
      std::cout << "========================================================" << std::endl;

    }
  }
}

TEST_CASE("Integration test from SP Parser to PKB") {
  GIVEN("An array of tokens representing the simple program") {
    std::vector<Token> tokens = {
      // procedure main {
      Token(SP::TokenType::KEYWORD_PROCEDURE, "procedure"),
      Token(SP::TokenType::NAME, "main"),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 1. x = v + x * y + z * t;
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::NAME, "v"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::TIMES, "*"),
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::NAME, "z"),
      Token(SP::TokenType::TIMES, "*"),
      Token(SP::TokenType::NAME, "t"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // 2. if ((x > 2) || ((y == 1) && (!(x == 0)))) then {
      Token(SP::TokenType::KEYWORD_IF, "if"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::GREATER_THAN, ">"),
      Token(SP::TokenType::INTEGER, "2"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::OR, "||"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::DOUBLE_EQUAL, "=="),
      Token(SP::TokenType::INTEGER, "1"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::AND, "&&"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NOT, "!"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::DOUBLE_EQUAL, "=="),
      Token(SP::TokenType::INTEGER, "0"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::KEYWORD_THEN, "then"),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 3. x = v + x * y + z * t;
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::NAME, "v"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::TIMES, "*"),
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::NAME, "z"),
      Token(SP::TokenType::TIMES, "*"),
      Token(SP::TokenType::NAME, "t"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // 4. y = 1 + 3 / 5 % x;
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::INTEGER, "1"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::INTEGER, "3"),
      Token(SP::TokenType::SLASH, "/"),
      Token(SP::TokenType::INTEGER, "5"),
      Token(SP::TokenType::PERCENT, "%"),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // 5. if (x>2) then {
      Token(SP::TokenType::KEYWORD_IF, "if"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::GREATER_THAN, ">"),
      Token(SP::TokenType::INTEGER, "2"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::KEYWORD_THEN, ""),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 6. x=1;
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::INTEGER, "1"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // } else {
      Token(SP::TokenType::RIGHT_BRACE, "}"),
      Token(SP::TokenType::KEYWORD_ELSE, "else"),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 8. a = (x % y) + ((y / 1) * 3);
      Token(SP::TokenType::NAME, "a"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "x"),
      Token(SP::TokenType::PERCENT, "%"),
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::LEFT_PAREN, "("),
      Token(SP::TokenType::NAME, "y"),
      Token(SP::TokenType::SLASH, "/"),
      Token(SP::TokenType::INTEGER, "1"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::TIMES, "*"),
      Token(SP::TokenType::INTEGER, "3"),
      Token(SP::TokenType::RIGHT_PAREN, ")"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // };
      Token(SP::TokenType::RIGHT_BRACE, "}"),

      // } else {
      Token(SP::TokenType::RIGHT_BRACE, "}"),
      Token(SP::TokenType::KEYWORD_ELSE, "else"),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 7. a = 5;
      Token(SP::TokenType::NAME, "a"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::INTEGER, "5"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // }
      Token(SP::TokenType::RIGHT_BRACE, "}"),

      // }
      Token(SP::TokenType::RIGHT_BRACE, "}"),

      // procedure main {
      Token(SP::TokenType::KEYWORD_PROCEDURE, "procedure"),
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::LEFT_BRACE, "{"),

      // 8. avg = num1 + num2;
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::NAME, "num1"),
      Token(SP::TokenType::PLUS, "+"),
      Token(SP::TokenType::NAME, "num2"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // 9. avg = avg / 2;
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::EQUAL, "="),
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::SLASH, "/"),
      Token(SP::TokenType::INTEGER, "2"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // 10. print avg;
      Token(SP::TokenType::KEYWORD_PRINT, "print"),
      Token(SP::TokenType::NAME, "avg"),
      Token(SP::TokenType::SEMICOLON, ";"),

      // }
      Token(SP::TokenType::RIGHT_BRACE, "}"),

      Token(SP::TokenType::EOFILE, ""),
    };

    // Initialise components
    // SP parser
    Parser parser(tokens);
    // PKB
    auto db = std::make_shared<Database>();
    std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);

    WHEN("The parser attempts to parse the expression") {
      auto root = parser.parse();

      THEN("It should not return nullptr") {
        REQUIRE(root != nullptr);
      }

      AND_WHEN("The design extractor extracts relationships") {
        // SP extractor
        DesignExtractor extractor(pkbFacade);
        extractor.extractDesign(*root);
        THEN("The PKB should store the correct entities and variables") {
          // Check procedures
          std::vector<std::string> expectedProcs = {"main", "avg"};
          std::vector<std::string> actualProcs = pkbFacade->getAllProcedures();
          REQUIRE(checkVecValuesEqual(expectedProcs, actualProcs));

          // Check variables
          std::vector<std::string> expectedValues = {"a", "t", "v", "x", "y", "z", "avg", "num1", "num2"};
          std::vector<std::string> actualValues = pkbFacade->getAllVariables();
          REQUIRE(checkVecValuesEqual(expectedValues, actualValues));

          // Check literals
          std::vector<int> expectedliterals = {0, 1, 2, 3, 5};
          std::vector<int> actualLiterals = pkbFacade->getAllConstants();
          REQUIRE(checkVecValuesEqual(expectedliterals, actualLiterals));

          // Check statements
          std::vector<int> expectedStmts = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
          std::vector<int> actualStmts = pkbFacade->getAllStatementNum();
          REQUIRE(checkVecValuesEqual(expectedStmts, actualStmts));

          // Check if statements
          std::vector<int> expectedIfStmts = {2, 5};
          std::vector<int> actualIfStmts = pkbFacade->getAllIfStmtNum();
          REQUIRE(checkVecValuesEqual(expectedIfStmts, actualIfStmts));

          // Check print statements
          std::vector<int> expectedPrintStmts = {11};
          std::vector<int> actualPrintStmts = pkbFacade->getAllPrintStmtNum();
          REQUIRE(checkVecValuesEqual(expectedPrintStmts, actualPrintStmts));

          // Check assignment statements
          std::vector<int> expectedAssignStmts = {1, 3, 4, 6, 7, 8, 9, 10};
          std::vector<int> actualAssignStmts = pkbFacade->getAllAssignStmtNum();
          REQUIRE(checkVecValuesEqual(expectedAssignStmts, actualAssignStmts));

          // No read statements
          REQUIRE(pkbFacade->getAllReadStmtNum().empty());

          // No while statements
          REQUIRE(pkbFacade->getAllWhileStmtNum().empty());
        }
      }
    }
  }
}

TEST_CASE("Integration test SP to PKB with keywords used as name") {
  std::string testProgram = "procedure test {"
    "if (less < b + c * d) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "while = 10;"
    "then = while * 3;"
    "if (1 < more) then {"
    "if = then * 5;"
    "} else {"
    "else = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    SPFacade.populatePKB();
    THEN("The PKB should store the correct entities and variables") {
      // Check procedures
      std::vector<std::string> expectedProcs = {"test"};
      std::vector<std::string> actualProcs = pkbFacade->getAllProcedures();
      REQUIRE(checkVecValuesEqual(expectedProcs, actualProcs));

      // Check variables
      std::vector<std::string> expectedValues = {
        "hello", "world", "less", "b", "c", "d", "more",
        "if", "then", "else", "while"
      };
      std::vector<std::string> actualValues = pkbFacade->getAllVariables();
      REQUIRE(checkVecValuesEqual(expectedValues, actualValues));

      // Check literals
      std::vector<int> expectedliterals = {1, 3, 5, 10};
      std::vector<int> actualLiterals = pkbFacade->getAllConstants();
      REQUIRE(checkVecValuesEqual(expectedliterals, actualLiterals));

      // Check statements
      std::vector<int> expectedStmts = {1, 2, 3, 4, 5, 6, 7, 8};
      std::vector<int> actualStmts = pkbFacade->getAllStatementNum();
      REQUIRE(checkVecValuesEqual(expectedStmts, actualStmts));

      // Check if statements
      std::vector<int> expectedIfStmts = {1, 6};
      std::vector<int> actualIfStmts = pkbFacade->getAllIfStmtNum();
      REQUIRE(checkVecValuesEqual(expectedIfStmts, actualIfStmts));

      // Check print statements
      std::vector<int> expectedPrintStmts = {3};
      std::vector<int> actualPrintStmts = pkbFacade->getAllPrintStmtNum();
      REQUIRE(checkVecValuesEqual(expectedPrintStmts, actualPrintStmts));

      // Check assignment statements
      std::vector<int> expectedAssignStmts = {2, 4, 5, 7, 8};
      std::vector<int> actualAssignStmts = pkbFacade->getAllAssignStmtNum();
      REQUIRE(checkVecValuesEqual(expectedAssignStmts, actualAssignStmts));

      // No read statements
      REQUIRE(pkbFacade->getAllReadStmtNum().empty());

      // No while statements
      REQUIRE(pkbFacade->getAllWhileStmtNum().empty());
    }
  }
}

TEST_CASE("Integration test SP to PKB with invalid extraneous brackets surrounding cond_expr") {
  std::string testProgram = "procedure test {"
    "if ((less < b + c * d)) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "hoho = 10;"
    "hehe = haha * 3;"
    "if (1 < more) then {"
    "huhu = hihi * 5;"
    "} else {"
    "hbhb = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    THEN("The processing should fail") {
      REQUIRE_THROWS(SPFacade.populatePKB());
    }
  }
}

TEST_CASE("Integration test SP to PKB with excessive extraneous brackets surrounding cond_expr") {
  std::string testProgram = "procedure test {"
    "if ((((((less < b + c * d)))))) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "hoho = 10;"
    "hehe = haha * 3;"
    "if (1 < more) then {"
    "huhu = hihi * 5;"
    "} else {"
    "hbhb = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    THEN("The processing should fail") {
      REQUIRE_THROWS(SPFacade.populatePKB());
    }
  }
}

TEST_CASE("Integration test SP to PKB with wrongly used negation and brackets surrounding cond_expr") {
  std::string testProgram = "procedure test {"
    "if ((a < b) || !(5 < 15 || 5 > 15)) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "hoho = 10;"
    "hehe = haha * 3;"
    "if (1 < more) then {"
    "huhu = hihi * 5;"
    "} else {"
    "hbhb = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    THEN("The processing should fail") {
      REQUIRE_THROWS(SPFacade.populatePKB());
    }
  }
}

TEST_CASE("Integration test SP to PKB with valid extraneous negations surrounding cond_expr") {
  std::string testProgram = "procedure test {"
    "if (!(!(!(!(i < like + testing * exclamation))))) then {"
    "hello = (5 + 10) / 3;"
    "print world;"
    "} else {"
    "hoho = 10;"
    "hehe = haha * 3;"
    "if (1 < more) then {"
    "huhu = hihi * 5;"
    "} else {"
    "hbhb = if / 3;"
    "}"
    "}"
    "}";

  auto db = std::make_shared<Database>();
  std::shared_ptr<PKBFacade> pkbFacade = std::make_shared<PKBFacade>(db);
  SPFacade SPFacade(pkbFacade, testProgram);

  WHEN("The SP processes the program expression") {
    THEN("The processing should not throw any exception") {
      REQUIRE_NOTHROW(SPFacade.populatePKB());
    }
  }
}
