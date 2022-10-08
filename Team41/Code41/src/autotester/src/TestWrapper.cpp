#include <fstream>
#include <sstream>
#include "TestWrapper.h"
#include "query_builder/QueryBuilder.h"
#include "query_builder/exceptions/Exceptions.h"
#include "query_evaluator/QueryEvaluator.h"
#include "parser/ASTBuilder.h"
#include "parser/Parser.h"
#include "parser/Tokenizer.h"
#include "design_extractor/DesignExtractor.h"
#include "query_evaluator/DataPreprocessor.h"

using namespace QB;
using namespace QE;
using namespace SourceParser;
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
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    // call your parser to do the parsing
    // ...rest of your code...
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    spaManager = make_shared<SPAManager>(pkbStorage);
    spaManager->parse(filename);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
    // ...code to evaluate query...
    vector<string> queryResults = spaManager->evaluate(query);
    for (const auto& result: queryResults) {
        results.push_back(result);
    }
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
}

TestWrapper::~TestWrapper() {

}