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
        shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual = ASTUtils::getNodePtrToLineNumMap(
                programNode->procedureList[0]);
        unordered_map<shared_ptr<ASTNode>, int> expected =  unordered_map<shared_ptr<ASTNode>, int>{};
        REQUIRE(*actual == expected);
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
        shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual = ASTUtils::getNodePtrToLineNumMap(
                programNode->procedureList[0]);
        unordered_map<shared_ptr<ASTNode>, int> expected =  unordered_map<shared_ptr<ASTNode>, int>{{printNode, 1}, {readNode, 2}};
        REQUIRE(*actual == expected);
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

        shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual = ASTUtils::getNodePtrToLineNumMap(
                programNode->procedureList[0]);
        unordered_map<shared_ptr<ASTNode>, int> expected =  unordered_map<shared_ptr<ASTNode>, int>{{printNode, 1}, {readNode, 2}, {ifNode, 3}, {printNode2, 4} , {readNode2 , 5}, {readNode3, 6}};
        REQUIRE(*actual == expected);
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

        shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> actual = ASTUtils::getNodePtrToLineNumMap(
                programNode->procedureList[0]);
        unordered_map<shared_ptr<ASTNode>, int> expected =  unordered_map<shared_ptr<ASTNode>, int>{
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
        REQUIRE(*actual == expected);
    }
}