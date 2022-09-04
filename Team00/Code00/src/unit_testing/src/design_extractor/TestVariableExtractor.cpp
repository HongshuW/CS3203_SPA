//
// Created by Aaron on 4/9/22.
//

#include "catch.hpp"
#include "iostream"

#include "parser/Parser.h"
#include "DesignExtractor.h"

using SourceParser::Parser;
using namespace std;

TEST_CASE("EXTRACT VARIABLES") {
    // create simple program node from wei en
    auto assign = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                                          make_shared<NumberNode>("1"));
    auto procedure = make_shared<ProcedureNode>("testProcedure", assign);
    vector<std::shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
            {procedure});
    auto expectedProgram = make_shared<ProgramNode>(procedureList);
    //end

    ProgramNode* programNode = expectedProgram.get();
    DesignExtractor designExtractor = DesignExtractor(*programNode);
    list<string> extractorList = designExtractor.extractVariables();
    REQUIRE(extractorList.front() == "x");
}
