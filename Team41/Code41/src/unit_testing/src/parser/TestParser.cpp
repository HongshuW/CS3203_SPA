//
// Created by Tan Wei En on 3/9/22.
//

#include "catch.hpp"
#include "iostream"

#include "parser/Parser.h"

using SourceParser::Parser;
using namespace std;

TEST_CASE("Test Parser") {

    // to change test cases, "hard code"
    SECTION("Simple source program, 1 assign statement") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "x", "=", "1", ";", "}"});
        Parser parser = Parser(tokens);
        auto testProgram = parser.parse();

        // simple program node
        auto assign = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                                              make_shared<NumberNode>("1"));
        auto procedure = make_shared<ProcedureNode>("testProcedure", assign);
        vector<std::shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        auto expectedProgram = make_shared<ProgramNode>(procedureList);
//        REQUIRE(*testProgram == *expectedProgram);
    }
}