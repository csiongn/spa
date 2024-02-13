#include <iostream>
#include <fstream>
#include <sstream>

#include "SPA.h"
#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  spa = SPA();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
    std::string fileContents;

    try {
        // Read the file contents into a string
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        fileContents = buffer.str();
    }
    catch (const std::exception& e) {
        std::cout << "Exception caught while reading file '" << filename << "': " << e.what() << std::endl;
        return; // Exit the function if an exception occurs while reading the file
    }

    try {
        spa.parseSimple(fileContents);
    }
    catch (const std::exception& e) {
        std::cout << "Exception caught while parsing file '" << filename << "': " << e.what() << std::endl;
    }

}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query..
  spa.evaluateQuery(query, results);
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
