//
// Created by Nafour on 11/9/22.
//
#include "catch.hpp"
#include "catch.hpp"
#include <unordered_map>
#include "design_extractor/DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "Dummies.h"
#include "DEUtils.h"
#include <string>
using namespace std;
using namespace DE;

TEST_CASE("Test pattern extraction")
{

    SECTION("test empty procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractPatterns();
        REQUIRE(actual.empty());
    }
    SECTION("test non-nested procedure") {
        /*
      * procedure2 {
      * 1 print x
      * 2 read y
      * 3 x = y + 1
      * }
      */
        shared_ptr<StmtNode> printNode_2 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode_2 = make_shared<ReadNode>(make_shared<VariableNode>("y"));
        shared_ptr<AssignNode> assignNode_2 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("y + 1"));
        shared_ptr<ProcedureNode> procedureNode2 = make_shared<ProcedureNode>(ProcedureNode("procedure1", {printNode_2, readNode_2, assignNode_2}));
        shared_ptr<ProgramNode> programNode2 = make_shared<ProgramNode>(ProgramNode({procedureNode2}));

        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode2);
        auto actual = designExtractor.extractPatterns();

        vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>> expected = {pair(pair(3, "x"), assignNode_2)};
        REQUIRE(actual.size() == expected.size());
        REQUIRE(TestDE::DEUtils::isSamePattern(actual, expected));
    }

    SECTION("test singly nested procedure") {
            /*
      * procedure3 {
      * 1 print x
      * 2 read y
      * 3 if (bar == y + 112312341234) then {
      * 4 x = y + 1
      * 5 print z} else {
      * 6 w = x + 3 }
      * 7 y = w + 1
      * }
      */
        shared_ptr<StmtNode> printNode_p3 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode_p3 = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> ifCondExpr_p3 = make_shared<CondExprNode>("bar == y + 112312341234");
        shared_ptr<StmtNode> printNode2_p3 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<AssignNode> assignNode2_p3 = make_shared<AssignNode>(make_shared<VariableNode>("w"), make_shared<ExprNode>("x + 3"));
        shared_ptr<AssignNode> assignNode_p3 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("y + 1"));
        vector<shared_ptr<StmtNode>> ifStmtList_p3 = {assignNode_p3, printNode2_p3};
        vector<shared_ptr<StmtNode>> elseStmtList_p3 = {assignNode2_p3};
        shared_ptr<StmtNode> ifNode_p3 = make_shared<IfNode>(ifCondExpr_p3, ifStmtList_p3, elseStmtList_p3);

        shared_ptr<AssignNode> assignNode3_p3 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("w + 1"));

        shared_ptr<ProcedureNode> procedureNode3 = make_shared<ProcedureNode>(ProcedureNode("procedure2", {printNode_p3, readNode_p3, ifNode_p3, assignNode3_p3}));
        shared_ptr<ProgramNode> programNode3 = make_shared<ProgramNode>(ProgramNode({procedureNode3}));

        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode3);
        auto actual = designExtractor.extractPatterns();

        vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>> expected = {
                pair(pair(4, "x"), assignNode_p3),
                pair(pair(6, "w"), assignNode2_p3),
                pair(pair(7, "y"),assignNode3_p3)
        };
        REQUIRE(actual.size() == expected.size());
        REQUIRE(TestDE::DEUtils::isSamePattern(actual, expected));
    }

    SECTION("test doubly nested procedure") {
        /*
       * procedure5 {
       * 1 x = 1
       * 2 while (x != 0) {
       * 3     x = 0
       * 4     if (x == 0) then {
       * 5         y = 1
       *       } else {
       * 6         y = 0
       *       }
       *   }
       * 7 y = 2
       * }
       */
        // 1
        shared_ptr<AssignNode> assignNode1_p5 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("1"));
        // 3
        shared_ptr<AssignNode> assignNode2_p5 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("0"));
        // 5
        shared_ptr<AssignNode> assignNode3_p5 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("1"));
        // 6
        shared_ptr<AssignNode> assignNode4_p5 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("0"));
        // 7
        shared_ptr<AssignNode> assignNode5_p5 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("2"));
        // 4
        vector<shared_ptr<StmtNode>> ifStmtLst_p5{assignNode3_p5};
        vector<shared_ptr<StmtNode>> elseStmtLst_p5{assignNode4_p5};
        shared_ptr<IfNode> ifNode_p5 = make_shared<IfNode>(make_shared<CondExprNode>("x == 0"), ifStmtLst_p5, elseStmtLst_p5);
        // 2
        vector<shared_ptr<StmtNode>> whileStmtLst_p5{assignNode2_p5, ifNode_p5};
        shared_ptr<WhileNode> whileNode_p5 = make_shared<WhileNode>(make_shared<CondExprNode>("x != 0"), whileStmtLst_p5);
        // procedure & program
        shared_ptr<ProcedureNode> procedureNode5 = make_shared<ProcedureNode>(ProcedureNode("CASE2", {assignNode1_p5, whileNode_p5, assignNode5_p5}));
        shared_ptr<ProgramNode> programNode5 = make_shared<ProgramNode>(ProgramNode({procedureNode5}));

        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode5);
        auto actual = designExtractor.extractPatterns();

        vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>> expected = {
              pair(pair(1, "x"), assignNode1_p5),
              pair(pair(3, "x"), assignNode2_p5),
              pair(pair(5, "y"), assignNode3_p5),
              pair(pair(6, "y"), assignNode4_p5),
              pair(pair(7, "y"), assignNode5_p5)
        };
        REQUIRE(actual.size() == expected.size());
        REQUIRE(TestDE::DEUtils::isSamePattern(actual, expected));
    }

}


