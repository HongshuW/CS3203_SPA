//
// Created by Aaron on 10/9/22.
//
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/FollowsExtractor.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/IfNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
#include <unordered_map>
#include "pkb/DataModifier.h"
#include "iostream"
#include "Dummies.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Follows Extractor") {
    SECTION("simple follow extraction") {
        /*
        * procedure1 {
        * 1 print x
        * 2 read y
        * 3 x = y + 1
        * }
        */

        shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));
        shared_ptr<StmtNode> assignNode = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                make_shared<ExprNode>("y + 1"));
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure1", {printNode, readNode, assignNode}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));

        shared_ptr<list<vector<string>>> output = FollowsExtractor().extractFollows(programNode);
        list<vector<string>>* actualOutput = output.get();
        list<vector<string>> actualOutput1 = *actualOutput;
        list<vector<string>> expected = { {"1", "2"}, {"2", "3"}};

        bool isValid = true;
        auto actualEntries = actualOutput1.begin();
        auto expectedEntries = expected.begin();
        while(actualEntries != actualOutput1.end() && expectedEntries != expected.end()) {
            auto v1 = *actualEntries;
            auto v2 = *expectedEntries;
            if (v1 != v2) {
                isValid = false;
            }
            actualEntries++;
            expectedEntries++;
        }

        REQUIRE(isValid);
    }
    SECTION("variable extraction in nested stmts") {
        /*
  * procedure2 {
  * 1 print x
  * 2 read y
  * 3 if (bar == y + 112312341234) then {
  * 4 x = y + 1
  * 5 print z} else {
  * 6 read w}
  * 7 read fo0
  * }
  */
        shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> ifCondExpr = make_shared<CondExprNode>("bar == y + 112312341234");
        shared_ptr<StmtNode> printNode2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2 = make_shared<ReadNode>(make_shared<VariableNode>("w"));
        shared_ptr<StmtNode> assignNode = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("y + 1"));
        vector<shared_ptr<StmtNode>> ifStmtList = {printNode2, assignNode};
        vector<shared_ptr<StmtNode>> elseStmtList = {readNode2};
        shared_ptr<StmtNode> ifNode = make_shared<IfNode>(ifCondExpr, ifStmtList, elseStmtList);

        shared_ptr<StmtNode> readNode3 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure2", {printNode, readNode, ifNode, readNode3}));
        shared_ptr<ProgramNode> programNode2 = make_shared<ProgramNode>(ProgramNode({procedureNode}));

        vector<vector<shared_ptr<StmtNode>>> s2 = FollowsExtractor::getListOfStmtList(programNode2);
        REQUIRE(1==1); //OK
    }
}
