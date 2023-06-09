//
// Created by Nafour on 9/9/22.
//
#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
#include "catch.hpp"
#include "design_extractor/extractors/ConstantExtractor.h"
#include "design_extractor/extractors/ProcedureExtractor.h"
#include "design_extractor/extractors/VariableExtractor.h"
#include "design_extractor/results/EntityResult.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;
TEST_CASE("Test entity extraction") {
  SECTION("simple variable extraction") {
    /*
     * procedure1 {
     * 1 print x
     * 2 read y
     * 3 x = y + 1
     * }
     */
    shared_ptr<StmtNode> printNode =
        make_shared<PrintNode>(make_shared<VariableNode>("x"));
    shared_ptr<StmtNode> readNode =
        make_shared<ReadNode>(make_shared<VariableNode>("y"));
    auto expressionNode = make_shared<ExprNode>("+");
    expressionNode->left = make_shared<ExprNode>("y");
    expressionNode->right = make_shared<ExprNode>("1");
    shared_ptr<StmtNode> assignNode =
        make_shared<AssignNode>(make_shared<VariableNode>("x"), expressionNode);
    shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(
        ProcedureNode("procedure1", {printNode, readNode, assignNode}));
    shared_ptr<ProgramNode> programNode =
        make_shared<ProgramNode>(ProgramNode({procedureNode}));
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    VariableExtractor variableExtractor =
        VariableExtractor(dataModifier, programNode);
    auto variableResult = variableExtractor.extract();
    auto varEntityResult = static_pointer_cast<EntityResult>(variableResult);
    auto actual = varEntityResult->getResult();
    list<string> variables_expected = list<string>{"x", "y"};
    REQUIRE(variables_expected == *actual);

    ConstantExtractor constantExtractor =
        ConstantExtractor(dataModifier, programNode);

    auto constantResult = constantExtractor.extract();
    auto constantEntityResult =
        static_pointer_cast<EntityResult>(constantResult);
    auto constants_actual = constantEntityResult->getResult();
    list<string> constants_expected = list<string>{"1"};
    REQUIRE(constants_expected == *constants_actual);

    ProcedureExtractor procedureExtractor =
        ProcedureExtractor(dataModifier, programNode);

    auto procedures_actual =
        static_pointer_cast<EntityResult>(procedureExtractor.extract())
            ->getResult();
    list<string> procedures_expected = list<string>{"procedure1"};
    REQUIRE(constants_expected == *constants_actual);
  }
  SECTION("variable extraction in nested stmts") {
    /*
     * procedure2 {
     * 1 print x
     * 2 read y
     * 3 if (bar == y + 112312341234) then {
     * 4 x = y + 10 - z
     * 5 print z} else {
     * 6 read w}
     * 7 read foo
     * }
     */
    shared_ptr<StmtNode> printNode =
        make_shared<PrintNode>(make_shared<VariableNode>("x"));
    shared_ptr<StmtNode> readNode =
        make_shared<ReadNode>(make_shared<VariableNode>("y"));
    auto rhs_p3 = make_shared<ExprNode>("+");
    rhs_p3->left = make_shared<ExprNode>("y");
    rhs_p3->right = make_shared<ExprNode>("112312341234");
    shared_ptr<CondExprNode> ifCondExpr = make_shared<CondExprNode>(
        make_shared<RelExprNode>(make_shared<ExprNode>("bar"), "==", rhs_p3));
    shared_ptr<StmtNode> printNode2 =
        make_shared<PrintNode>(make_shared<VariableNode>("z"));
    shared_ptr<StmtNode> readNode2 =
        make_shared<ReadNode>(make_shared<VariableNode>("w"));

    auto expressionNode = make_shared<ExprNode>("-");
    expressionNode->left = make_shared<ExprNode>("+");
    expressionNode->left->left = make_shared<ExprNode>("y");
    expressionNode->left->right = make_shared<ExprNode>("10");
    expressionNode->right = make_shared<ExprNode>("exprVar");

    shared_ptr<StmtNode> assignNode =
        make_shared<AssignNode>(make_shared<VariableNode>("x"), expressionNode);
    vector<shared_ptr<StmtNode>> ifStmtList = {printNode2, assignNode};
    vector<shared_ptr<StmtNode>> elseStmtList = {readNode2};
    shared_ptr<StmtNode> ifNode =
        make_shared<IfNode>(ifCondExpr, ifStmtList, elseStmtList);

    shared_ptr<StmtNode> readNode3 =
        make_shared<ReadNode>(make_shared<VariableNode>("foo"));

    shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(
        ProcedureNode("procedure2", {printNode, readNode, ifNode, readNode3}));
    shared_ptr<ProgramNode> programNode2 =
        make_shared<ProgramNode>(ProgramNode({procedureNode}));

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);

    VariableExtractor varExtractor =
        VariableExtractor(dataModifier, programNode2);
    auto variables_actual =
        static_pointer_cast<EntityResult>(varExtractor.extract())->getResult();
    list<string> variables_expected =
        list<string>{"x", "y", "w", "z", "foo", "bar", "exprVar"};
    variables_actual->sort();
    variables_expected.sort();
    REQUIRE(variables_expected == *variables_actual);

    ConstantExtractor constExtractor =
        ConstantExtractor(dataModifier, programNode2);
    auto constants_actual =
        static_pointer_cast<EntityResult>(constExtractor.extract())
            ->getResult();
    list<string> constants_expected = list<string>{"10", "112312341234"};
    constants_actual->sort();
    constants_expected.sort();
    REQUIRE(constants_expected == *constants_actual);
    ProcedureExtractor procedureExtractor =
        ProcedureExtractor(dataModifier, programNode2);

    auto procedures_actual =
        static_pointer_cast<EntityResult>(procedureExtractor.extract())
            ->getResult();
    procedures_actual->sort();

    list<string> procedures_expected = list<string>{"procedure2"};
    procedures_expected.sort();
    REQUIRE(constants_expected == *constants_actual);
  }
  SECTION("Test variable extraction in multiple nested stmts") {
    /*
     * procedure3 {
     * 1 print x
     * 2 read y
     * 3 if (bar == y + 1) then {
     * 4 print z
     * 5 while ((baz == qux + 1) && (5 + 3 < quux) || haha == 1) {
     * 6    read baz
     * 7    print bar
     *   }} else {
     * 8 read w}
     * 9 read foo
     * }
     */
    shared_ptr<StmtNode> printNode =
        make_shared<PrintNode>(make_shared<VariableNode>("x"));
    shared_ptr<StmtNode> readNode =
        make_shared<ReadNode>(make_shared<VariableNode>("y"));

    auto rhs1_p4 = make_shared<ExprNode>("+");
    rhs1_p4->left = make_shared<ExprNode>("y");
    rhs1_p4->right = make_shared<ExprNode>("1");
    shared_ptr<CondExprNode> ifCondExpr = make_shared<CondExprNode>(
        make_shared<RelExprNode>(make_shared<ExprNode>("bar"), "==", rhs1_p4));
    shared_ptr<StmtNode> printNode2 =
        make_shared<PrintNode>(make_shared<VariableNode>("z"));
    shared_ptr<StmtNode> readNode2 =
        make_shared<ReadNode>(make_shared<VariableNode>("w"));

    shared_ptr<StmtNode> printNode4 =
        make_shared<PrintNode>(make_shared<VariableNode>("bar"));
    shared_ptr<StmtNode> readNode4 =
        make_shared<ReadNode>(make_shared<VariableNode>("baz"));

    vector<shared_ptr<StmtNode>> whileStmtList = {readNode4, printNode4};
    shared_ptr<StmtNode> whileNode =
        make_shared<WhileNode>(ifCondExpr, whileStmtList);

    vector<shared_ptr<StmtNode>> ifStmtList = {printNode2, whileNode};
    vector<shared_ptr<StmtNode>> elseStmtList = {readNode2};

    shared_ptr<ExprNode> baz_p4 = make_shared<ExprNode>("baz");
    shared_ptr<ExprNode> qux_p4 = make_shared<ExprNode>("qux");
    shared_ptr<ExprNode> quux_p4 = make_shared<ExprNode>("quux");
    shared_ptr<ExprNode> haha_p4 = make_shared<ExprNode>("haha");
    shared_ptr<ExprNode> qux_plus_1_p4 = make_shared<ExprNode>("+");
    qux_plus_1_p4->left = qux_p4;
    qux_plus_1_p4->right = make_shared<ExprNode>("1");
    shared_ptr<ExprNode> five_plus_three = make_shared<ExprNode>("+");
    five_plus_three->left = make_shared<ExprNode>("5");
    five_plus_three->right = make_shared<ExprNode>("3");

    shared_ptr<CondExprNode> whileCondExprChild1 = make_shared<CondExprNode>(
        make_shared<CondExprNode>(
            make_shared<RelExprNode>(baz_p4, "==", qux_plus_1_p4)),
        "&&",
        make_shared<CondExprNode>(
            make_shared<RelExprNode>(five_plus_three, "<", quux_p4)));
    shared_ptr<CondExprNode> whileCondExprChild2 = make_shared<CondExprNode>(
        make_shared<RelExprNode>(haha_p4, "==", make_shared<ExprNode>("1")));
    shared_ptr<CondExprNode> whileCondExpr = make_shared<CondExprNode>(
        whileCondExprChild1, "||", whileCondExprChild2);
    shared_ptr<StmtNode> ifNode =
        make_shared<IfNode>(whileCondExpr, ifStmtList, elseStmtList);

    shared_ptr<StmtNode> readNode3 =
        make_shared<ReadNode>(make_shared<VariableNode>("foo"));

    shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(
        ProcedureNode("procedure3", {printNode, readNode, ifNode, readNode3}));
    shared_ptr<ProgramNode> programNode3 =
        make_shared<ProgramNode>(ProgramNode({procedureNode}));
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);

    VariableExtractor varExtractor =
        VariableExtractor(dataModifier, programNode3);
    auto variables_actual =
        static_pointer_cast<EntityResult>(varExtractor.extract())->getResult();
    list<string> variables_expected = list<string>{
        "x", "y", "w", "z", "foo", "bar", "baz", "qux", "quux", "haha"};
    variables_actual->sort();
    variables_expected.sort();
    REQUIRE(variables_expected == *variables_actual);

    ConstantExtractor constExtractor =
        ConstantExtractor(dataModifier, programNode3);
    auto constants_actual =
        static_pointer_cast<EntityResult>(constExtractor.extract())
            ->getResult();
    list<string> constants_expected = list<string>{"1", "5", "3"};
    constants_actual->sort();
    constants_expected.sort();
    REQUIRE(constants_expected == *constants_actual);
    ProcedureExtractor procedureExtractor =
        ProcedureExtractor(dataModifier, programNode3);
    auto procedures_actual =
        static_pointer_cast<EntityResult>(procedureExtractor.extract())
            ->getResult();

    list<string> procedures_expected = list<string>{"procedure3"};
    procedures_actual->sort();
    procedures_expected.sort();
    REQUIRE(constants_expected == *constants_actual);
  }
}