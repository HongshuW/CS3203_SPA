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
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/FollowsExtractor.h"
#include "pkb/DataModifier.h"
#include "iostream"
#include "Dummies.h"

using namespace std;
using namespace DE;
TEST_CASE("Test entity extraction") {
    SECTION("no variable ") {
        /*
        * procedure1 {
        * }
        */
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure1", {}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        auto variables_variables_actual = designExtractor.extractEntities(DesignEntity::VARIABLE);
        unordered_set<string> variables_variables_expected = unordered_set<string>{};
        REQUIRE(variables_variables_expected == *variables_variables_actual);

        auto constants_actual = designExtractor.extractEntities(DesignEntity::CONSTANT);
        unordered_set<string> constants_expected = unordered_set<string>{};
        REQUIRE(constants_expected == *constants_actual);

    }
    SECTION("simple variable extraction") {
        /*
        * procedure1 {
        * 1 print x
        * 2 read y
        * 3 x = y + 1
        * }
        */
        shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));
        shared_ptr<StmtNode> assignNode = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("y + 1"));
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure1", {printNode, readNode, assignNode}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        auto variables_actual = designExtractor.extractEntities(DesignEntity::VARIABLE);
        unordered_set<string> variables_expected = unordered_set<string>{"x", "y"};
        REQUIRE(variables_expected == *variables_actual);

        auto constants_actual = designExtractor.extractEntities(DesignEntity::CONSTANT);
        unordered_set<string> constants_expected = unordered_set<string>{"1"};
        REQUIRE(constants_expected == *constants_actual);

        auto procedures_actual = designExtractor.extractEntities(DesignEntity::PROCEDURE);
        unordered_set<string> procedures_expected = unordered_set<string>{"procedure1"};
        REQUIRE(constants_expected == *constants_actual);
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

        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier,programNode2);
        auto variables_actual = designExtractor.extractEntities(DesignEntity::VARIABLE);
        unordered_set<string> variables_expected = unordered_set<string>{"x", "y","w", "z", "foo", "bar"};
        REQUIRE(variables_expected == *variables_actual);

        auto constants_actual = designExtractor.extractEntities(DesignEntity::CONSTANT);
        unordered_set<string> constants_expected = unordered_set<string>{"1", "112312341234"};
        REQUIRE(constants_expected == *constants_actual);

        auto procedures_actual = designExtractor.extractEntities(DesignEntity::PROCEDURE);
        unordered_set<string> procedures_expected = unordered_set<string>{"procedure2"};
        REQUIRE(constants_expected == *constants_actual);
    }
    SECTION("Test variable extraction in multiple nested stmts") {
        /*
         * procedure3 {
         * 1 print x
         * 2 read y
         * 3 if (bar == y + 1) then {
         * 4 print z
         * 5 while ((baz == qux + 1) && (5 + 3 < quux) || haha) {
         * 6    read baz
         * 7    print bar
         *   }} else {
         * 8 read w}
         * 9 read foo
         * }
         */
        shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> ifCondExpr = make_shared<CondExprNode>("bar == y + 1");
        shared_ptr<StmtNode> printNode2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2 = make_shared<ReadNode>(make_shared<VariableNode>("w"));

        shared_ptr<StmtNode> printNode4 = make_shared<PrintNode>(make_shared<VariableNode>("bar"));
        shared_ptr<StmtNode> readNode4 = make_shared<ReadNode>(make_shared<VariableNode>("baz"));


        vector<shared_ptr<StmtNode>> whileStmtList = {readNode4, printNode4};
        shared_ptr<StmtNode> whileNode = make_shared<WhileNode>(ifCondExpr, whileStmtList);


        vector<shared_ptr<StmtNode>> ifStmtList = {printNode2, whileNode};
        vector<shared_ptr<StmtNode>> elseStmtList = {readNode2};

        shared_ptr<CondExprNode> whileCondExpr = make_shared<CondExprNode>("(baz == qux + 1) && (5 + 3 < quux) || haha");
        shared_ptr<StmtNode> ifNode = make_shared<IfNode>(whileCondExpr, ifStmtList, elseStmtList);

        shared_ptr<StmtNode> readNode3 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure3", {printNode, readNode, ifNode, readNode3}));
        shared_ptr<ProgramNode> programNode3 = make_shared<ProgramNode>(ProgramNode({procedureNode}));
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier,programNode3);
        auto variables_actual = designExtractor.extractEntities(DesignEntity::VARIABLE);
        unordered_set<string> variables_expected = unordered_set<string>{"x", "y","w", "z", "foo", "bar", "baz", "qux", "quux", "haha"};
        REQUIRE(variables_expected == *variables_actual);


        auto constants_actual = designExtractor.extractEntities(DesignEntity::CONSTANT);
        unordered_set<string> constants_expected = unordered_set<string>{"1", "5", "3"};
        REQUIRE(constants_expected == *constants_actual);

        auto procedures_actual = designExtractor.extractEntities(DesignEntity::PROCEDURE);
        unordered_set<string> procedures_expected = unordered_set<string>{"procedure3"};
        REQUIRE(constants_expected == *constants_actual);
    }
    SECTION("Test multiple procedures") {
        /*
   * procedure1 {
   * 1 print x
   * 2 read y
   * 3 if (bar == y + 1) then {
   * 4 print z} else {
   * 5 read w}
   * 6 read foo
   * }
   */
        shared_ptr<StmtNode> printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> ifCondExpr = make_shared<CondExprNode>("bar == y * 1");
        shared_ptr<StmtNode> printNode2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2 = make_shared<ReadNode>(make_shared<VariableNode>("w"));
        vector<shared_ptr<StmtNode>> ifStmtList = {printNode2};
        vector<shared_ptr<StmtNode>> elseStmtList = {readNode2};
        shared_ptr<StmtNode> ifNode = make_shared<IfNode>(ifCondExpr, ifStmtList, elseStmtList);

        shared_ptr<StmtNode> readNode3 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedureNode1 = make_shared<ProcedureNode>(ProcedureNode("procedure2", {printNode, readNode, ifNode, readNode3}));


        /*
         * procedure2 {
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
        shared_ptr<StmtNode> printNodeP2 = make_shared<PrintNode>(make_shared<VariableNode>("v1"));
        shared_ptr<StmtNode> readNodeP2 = make_shared<ReadNode>(make_shared<VariableNode>("v2"));

        shared_ptr<CondExprNode> ifCondExprP2 = make_shared<CondExprNode>("bar == y + 100 / 20");
        shared_ptr<StmtNode> printNode2P2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2P2 = make_shared<ReadNode>(make_shared<VariableNode>("w"));

        shared_ptr<StmtNode> printNode4P2 = make_shared<PrintNode>(make_shared<VariableNode>("bar"));
        shared_ptr<StmtNode> readNode4P2 = make_shared<ReadNode>(make_shared<VariableNode>("baz"));


        vector<shared_ptr<StmtNode>> whileStmtListP2 = {readNode4P2, printNode4P2};
        shared_ptr<StmtNode> whileNodeP2 = make_shared<WhileNode>(ifCondExprP2, whileStmtListP2);


        vector<shared_ptr<StmtNode>> ifStmtListP2 = {printNode2P2, whileNodeP2};
        vector<shared_ptr<StmtNode>> elseStmtListP2 = {readNode2P2};

        shared_ptr<CondExprNode> whileCondExprP2 = make_shared<CondExprNode>("(baz == qux + 1) && (5 + 3 <= quux) || haha => 4");
        shared_ptr<StmtNode> ifNodeP2 = make_shared<IfNode>(whileCondExprP2, ifStmtListP2, elseStmtListP2);

        shared_ptr<StmtNode> readNode3P2 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedureNode2 = make_shared<ProcedureNode>(ProcedureNode("procedure3", {printNodeP2, readNodeP2, ifNodeP2, readNode3P2}));

        shared_ptr<ProgramNode> programNode3 = make_shared<ProgramNode>(ProgramNode({procedureNode1, procedureNode2}));
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier,programNode3);
        auto variables_actual = designExtractor.extractEntities(DesignEntity::VARIABLE);
        unordered_set<string> variables_expected = unordered_set<string>{"x", "y","w", "z", "foo", "bar", "v1", "v2", "baz", "qux", "quux", "haha"};
        REQUIRE(variables_expected == *variables_actual);

        auto constants_actual = designExtractor.extractEntities(DesignEntity::CONSTANT);
        unordered_set<string> constants_expected = unordered_set<string>{"1", "100", "20", "5", "3", "4"};
        REQUIRE(constants_expected == *constants_actual);
<<<<<<< HEAD

        auto procedures_actual = designExtractor.extractEntities(DesignEntity::PROCEDURE);
        unordered_set<string> procedures_expected = unordered_set<string>{"procedure2", "procedure3"};
        REQUIRE(constants_expected == *constants_actual);
=======
>>>>>>> 1d3aba53714197bba8a183a16fb9e675ce3954ca
    }
}