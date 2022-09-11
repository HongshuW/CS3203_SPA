//
// Created by Nafour on 9/9/22.
//

#include "Dummies.h"

namespace TestDE {

    shared_ptr<ProgramNode> Dummies::getTestProgramNode(int idx) {

        /*
        * procedure1 {
        * }
        */
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("procedure1", {}));
        shared_ptr<ProgramNode> programNode1 = make_shared<ProgramNode>(ProgramNode({procedureNode}));




        //2
        /*
       * procedure2 {
       * 1 print x
       * 2 read y
       * 3 x = y + 1
       * }
       */
        shared_ptr<StmtNode> printNode_2 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode_2 = make_shared<ReadNode>(make_shared<VariableNode>("y"));
        shared_ptr<StmtNode> assignNode_2 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("y + 1"));
        shared_ptr<ProcedureNode> procedureNode2 = make_shared<ProcedureNode>(ProcedureNode("procedure1", {printNode_2, readNode_2, assignNode_2}));
        shared_ptr<ProgramNode> programNode2 = make_shared<ProgramNode>(ProgramNode({procedureNode2}));




            /*
       * procedure3 {
       * 1 print x
       * 2 read y
       * 3 if (bar == y + 112312341234) then {
       * 4 x = y + 1
       * 5 print z} else {
       * 6 read w}
       * 7 read fo0
       * }
       */
        shared_ptr<StmtNode> printNode_p3 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode_p3 = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> ifCondExpr_p3 = make_shared<CondExprNode>("bar == y + 112312341234");
        shared_ptr<StmtNode> printNode2_p3 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2_p3 = make_shared<ReadNode>(make_shared<VariableNode>("w"));
        shared_ptr<StmtNode> assignNode_p3 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("y + 1"));
        vector<shared_ptr<StmtNode>> ifStmtList_p3 = {assignNode_p3, printNode2_p3};
        vector<shared_ptr<StmtNode>> elseStmtList_p3 = {readNode2_p3};
        shared_ptr<StmtNode> ifNode_p3 = make_shared<IfNode>(ifCondExpr_p3, ifStmtList_p3, elseStmtList_p3);

        shared_ptr<StmtNode> readNode3_p3 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedureNode3 = make_shared<ProcedureNode>(ProcedureNode("procedure2", {printNode_p3, readNode_p3, ifNode_p3, readNode3_p3}));
        shared_ptr<ProgramNode> programNode3 = make_shared<ProgramNode>(ProgramNode({procedureNode3}));



        /*
         * procedure4 {
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
        shared_ptr<StmtNode> printNode_p4 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode_p4 = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> ifCondExpr_p4 = make_shared<CondExprNode>("bar == y + 1");
        shared_ptr<StmtNode> printNode2_p4 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2_p4 = make_shared<ReadNode>(make_shared<VariableNode>("w"));

        shared_ptr<StmtNode> printNode4_p4 = make_shared<PrintNode>(make_shared<VariableNode>("bar"));
        shared_ptr<StmtNode> readNode4_p4 = make_shared<ReadNode>(make_shared<VariableNode>("baz"));


        shared_ptr<CondExprNode> whileCondExpr_p4 = make_shared<CondExprNode>("(baz == qux + 1) && (5 + 3 < quux) || haha");
        vector<shared_ptr<StmtNode>> whileStmtList_p4 = {readNode4_p4, printNode4_p4};
        shared_ptr<StmtNode> whileNode_p4 = make_shared<WhileNode>(whileCondExpr_p4, whileStmtList_p4);


        vector<shared_ptr<StmtNode>> ifStmtList_p4 = {printNode2_p4, whileNode_p4};
        vector<shared_ptr<StmtNode>> elseStmtList_p4 = {readNode2_p4};

        shared_ptr<StmtNode> ifNode_p4 = make_shared<IfNode>(ifCondExpr_p4, ifStmtList_p4, elseStmtList_p4);

        shared_ptr<StmtNode> readNode3_p4 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedureNode4 = make_shared<ProcedureNode>(ProcedureNode("procedure3", {printNode_p4, readNode_p4, ifNode_p4, readNode3_p4}));
        shared_ptr<ProgramNode> programNode4 = make_shared<ProgramNode>(ProgramNode({procedureNode4}));

        vector<shared_ptr<ProgramNode>> programNodes = vector<shared_ptr<ProgramNode>>{programNode1, programNode2, programNode3, programNode4};

        return programNodes[idx];
    }

} // TestDE