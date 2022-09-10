//
// Created by Nafour on 9/9/22.
//

#include "catch.hpp"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/IfNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
#include <unordered_map>
using namespace std;

TEST_CASE("Test AST utils") {
    SECTION("Test line number map traversal with no stmt node") {
        /*
         * procedure1 {
         * }
         */
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure1", {}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));

        shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual_nodeToLine = make_shared<unordered_map<shared_ptr<ASTNode>, int>>();
        shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> actual_lineToNode = make_shared<unordered_map<int, shared_ptr<ASTNode>>>();
        ASTUtils::getNodePtrToLineNumMap(
                programNode, actual_nodeToLine, actual_lineToNode);

        ASTUtils::getNodePtrToLineNumMap(
                programNode, actual_nodeToLine, actual_lineToNode);

        unordered_map<shared_ptr<ASTNode>, int> expected_nodeToLine =  unordered_map<shared_ptr<ASTNode>, int>{};
        REQUIRE(*actual_nodeToLine == expected_nodeToLine);


        unordered_map<int, shared_ptr<ASTNode>> expected_lineToNode =  unordered_map<int, shared_ptr<ASTNode>>{};
        REQUIRE(expected_lineToNode == *actual_lineToNode);
    }
    SECTION("Test line number map traversal without nesting") {
        /*
         * procedure1 {
         * 1 print x
         * 2 read y
         * }
         */
        shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure1", {printNode, readNode}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));
        shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual_nodeToLine = make_shared<unordered_map<shared_ptr<ASTNode>, int>>();
        shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> actual_lineToNode = make_shared<unordered_map<int, shared_ptr<ASTNode>>>();
        ASTUtils::getNodePtrToLineNumMap(
                programNode, actual_nodeToLine, actual_lineToNode);

        ASTUtils::getNodePtrToLineNumMap(
                programNode, actual_nodeToLine, actual_lineToNode);
        unordered_map<shared_ptr<ASTNode>, int> expected_nodeToLine =  unordered_map<shared_ptr<ASTNode>, int>{{printNode, 1}, {readNode, 2}};


        REQUIRE(*actual_nodeToLine == expected_nodeToLine);

        unordered_map<int, shared_ptr<ASTNode>> expected_lineToNode =  unordered_map<int, shared_ptr<ASTNode>>{{1, printNode}, {2, readNode}};
        REQUIRE(expected_lineToNode == *actual_lineToNode);
    }
    SECTION("Test line number map traversal with single nesting") {
        /*
         * procedure2 {
         * 1 print x
         * 2 read y
         * 3 if (...) then {
         * 4 print z} else {
         * 5 read w}
         * 6 read foo
         * }
         */
        shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> condExpr;
        shared_ptr<StmtNode> printNode2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2 = make_shared<ReadNode>(make_shared<VariableNode>("w"));
        vector<shared_ptr<StmtNode>> ifStmtList = {printNode2};
        vector<shared_ptr<StmtNode>> elseStmtList = {readNode2};
        shared_ptr<StmtNode> ifNode = make_shared<IfNode>(condExpr, ifStmtList, elseStmtList);

        shared_ptr<StmtNode> readNode3 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure2", {printNode, readNode, ifNode, readNode3}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));

        shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual_nodeToLine = make_shared<unordered_map<shared_ptr<ASTNode>, int>>();
        shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> actual_lineToNode = make_shared<unordered_map<int, shared_ptr<ASTNode>>>();
        ASTUtils::getNodePtrToLineNumMap(
                programNode, actual_nodeToLine, actual_lineToNode);

        ASTUtils::getNodePtrToLineNumMap(
                programNode, actual_nodeToLine, actual_lineToNode);
        unordered_map<shared_ptr<ASTNode>, int> expected_nodeToLine =  unordered_map<shared_ptr<ASTNode>, int>{{printNode, 1}, {readNode, 2}, {ifNode, 3}, {printNode2, 4} , {readNode2 , 5}, {readNode3, 6}};
        REQUIRE(*actual_nodeToLine == expected_nodeToLine);

        unordered_map<int, shared_ptr<ASTNode>> expected_lineToNode =  unordered_map<int, shared_ptr<ASTNode>>{{1, printNode}, {2, readNode}, {3, ifNode}, {4, printNode2}, {5, readNode2}, {6, readNode3}};
        REQUIRE(expected_lineToNode == *actual_lineToNode);
    }
    SECTION("Test line number map traversal with double nesting") {
        /*
         * procedure3 {
         * 1 print x
         * 2 read y
         * 3 if (...) then {
         * 4 print z
         * 5 while (...) {
         * 6    read baz
         * 7    print bar
         *   }} else {
         * 8 read w}
         * 9 read foo
         * }
         */
        shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> condExpr;
        shared_ptr<StmtNode> printNode2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2 = make_shared<ReadNode>(make_shared<VariableNode>("w"));

        shared_ptr<StmtNode> printNode4 = make_shared<PrintNode>(make_shared<VariableNode>("bar"));
        shared_ptr<StmtNode> readNode4 = make_shared<ReadNode>(make_shared<VariableNode>("baz"));


        vector<shared_ptr<StmtNode>> whileStmtList = {readNode4, printNode4};
        shared_ptr<StmtNode> whileNode = make_shared<WhileNode>(condExpr, whileStmtList);


        vector<shared_ptr<StmtNode>> ifStmtList = {printNode2, whileNode};
        vector<shared_ptr<StmtNode>> elseStmtList = {readNode2};

        shared_ptr<StmtNode> ifNode = make_shared<IfNode>(condExpr, ifStmtList, elseStmtList);

        shared_ptr<StmtNode> readNode3 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure3", {printNode, readNode, ifNode, readNode3}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));

        shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual_nodeToLine = make_shared<unordered_map<shared_ptr<ASTNode>, int>>();
        shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> actual_lineToNode = make_shared<unordered_map<int, shared_ptr<ASTNode>>>();
        ASTUtils::getNodePtrToLineNumMap(
                programNode, actual_nodeToLine, actual_lineToNode);

        ASTUtils::getNodePtrToLineNumMap(
                programNode, actual_nodeToLine, actual_lineToNode);
        unordered_map<shared_ptr<ASTNode>, int> expected_nodeToLine =  unordered_map<shared_ptr<ASTNode>, int>{
            {printNode, 1},
            {readNode, 2},
            {ifNode, 3},
            {printNode2, 4},
            {whileNode , 5},
            {readNode4, 6},
            {printNode4, 7},
            {readNode2, 8},
            {readNode3, 9}
        };
        REQUIRE(*actual_nodeToLine == expected_nodeToLine);

        unordered_map<int, shared_ptr<ASTNode>> expected_lineToNode =  unordered_map<int, shared_ptr<ASTNode>>{
            {1, printNode},
            {2, readNode},
            {3, ifNode},
            {4, printNode2},
            {5, whileNode},
            {6, readNode4},
            {7, printNode4},
            {8, readNode2},
            {9, readNode3}};
        REQUIRE(expected_lineToNode == *actual_lineToNode);
    }
    SECTION("Test two procedure") {
            /*
             * procedure1 {
             * 1 print x
             * 2 read y
             * }
             */
            shared_ptr<StmtNode> printNode_p1 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
            shared_ptr<StmtNode> readNode_p1 = make_shared<ReadNode>(make_shared<VariableNode>("y"));
            shared_ptr<ProcedureNode> procedureNode_p1 = make_shared<ProcedureNode>(ProcedureNode("procedure1", {printNode_p1, readNode_p1}));


                /*
             * procedure2 {
             * 3 print x
             * 4 read y
             * 5 if (...) then {
             * 6 print z} else {
             * 7 read w}
             * 8 read foo
             * }
             */
            shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
            shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));

            shared_ptr<CondExprNode> condExpr;
            shared_ptr<StmtNode> printNode2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
            shared_ptr<StmtNode> readNode2 = make_shared<ReadNode>(make_shared<VariableNode>("w"));
            vector<shared_ptr<StmtNode>> ifStmtList = {printNode2};
            vector<shared_ptr<StmtNode>> elseStmtList = {readNode2};
            shared_ptr<StmtNode> ifNode = make_shared<IfNode>(condExpr, ifStmtList, elseStmtList);

            shared_ptr<StmtNode> readNode3 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

            shared_ptr<ProcedureNode> procedureNode2 = make_shared<ProcedureNode>(ProcedureNode("procedure2", {printNode, readNode, ifNode, readNode3}));


            shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode_p1, procedureNode2}));
            shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual_nodeToLine = make_shared<unordered_map<shared_ptr<ASTNode>, int>>();
            shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> actual_lineToNode = make_shared<unordered_map<int, shared_ptr<ASTNode>>>();
            ASTUtils::getNodePtrToLineNumMap(
                    programNode, actual_nodeToLine, actual_lineToNode);
            unordered_map<shared_ptr<ASTNode>, int> expected_nodeToLine =  unordered_map<shared_ptr<ASTNode>, int>{{printNode_p1, 1},
                                                                                                        {readNode_p1, 2},
                                                                                                        {printNode, 3},
                                                                                                        {readNode, 4},
                                                                                                        {ifNode, 5},
                                                                                                        {printNode2, 6} ,
                                                                                                        {readNode2 , 7},
                                                                                                        {readNode3, 8}};
            REQUIRE(*actual_nodeToLine == expected_nodeToLine);

        unordered_map<int, shared_ptr<ASTNode>> expected_lineToNode =  unordered_map<int, shared_ptr<ASTNode>>{
            {1, printNode_p1},
            {2, readNode_p1},
            {3, printNode},
            {4, readNode},
            {5, ifNode},
            {6, printNode2},
            {7, readNode2},
            {8,readNode3}
        };
    }
}