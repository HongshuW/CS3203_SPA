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
        shared_ptr<ExprNode> exprNode_2 = make_shared<ExprNode>("+");
        exprNode_2->left = make_shared<ExprNode>("y");
        exprNode_2->right = make_shared<ExprNode>("1");

        shared_ptr<StmtNode> assignNode_2 = make_shared<AssignNode>(make_shared<VariableNode>("x"), exprNode_2);
        shared_ptr<ProcedureNode> procedureNode2 = make_shared<ProcedureNode>(ProcedureNode("procedure1", {printNode_2, readNode_2, assignNode_2}));
        shared_ptr<ProgramNode> programNode2 = make_shared<ProgramNode>(ProgramNode({procedureNode2}));


      /*
       * procedure3 {
       * 1 print x
       * 2 read y
       * 3 if (bar == y + 112312341234) then {
       * 4     x = y + 1
       * 5     print z
       *   } else {
       * 6     read w
       *   }
       * 7 read fo0
       * }
       */
        shared_ptr<StmtNode> printNode_p3 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode_p3 = make_shared<ReadNode>(make_shared<VariableNode>("y"));

        shared_ptr<CondExprNode> ifCondExpr_p3 = make_shared<CondExprNode>("bar == y + 112312341234");
        shared_ptr<StmtNode> printNode2_p3 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2_p3 = make_shared<ReadNode>(make_shared<VariableNode>("w"));
        shared_ptr<ExprNode> exprNode_3 = make_shared<ExprNode>("+");
        exprNode_3->left = make_shared<ExprNode>("y");
        exprNode_3->right = make_shared<ExprNode>("1");

        shared_ptr<StmtNode> assignNode_p3 = make_shared<AssignNode>(make_shared<VariableNode>("x"), exprNode_3);
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
        shared_ptr<ProcedureNode> procedureNode5 = make_shared<ProcedureNode>(ProcedureNode("procedure5", {assignNode1_p5, whileNode_p5, assignNode5_p5}));
        shared_ptr<ProgramNode> programNode5 = make_shared<ProgramNode>(ProgramNode({procedureNode5}));

        /*
        * procedure6 {
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
        * 8 while (y != 0) {
        * 9     y = 0
        *   }
        * }
        */

        // 9
        shared_ptr<AssignNode> assignNode1_p6 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("0"));
        // 8
        vector<shared_ptr<StmtNode>> whileStmtLst_p6{assignNode1_p6};
        shared_ptr<WhileNode> whileNode_p6 = make_shared<WhileNode>(make_shared<CondExprNode>("y != 0"), whileStmtLst_p6);
        // procedure & program
        shared_ptr<ProcedureNode> procedureNode6 = make_shared<ProcedureNode>(ProcedureNode("procedure6", {assignNode1_p5, whileNode_p5, assignNode5_p5, whileNode_p6}));
        shared_ptr<ProgramNode> programNode6 = make_shared<ProgramNode>(ProgramNode({procedureNode6}));


        //multiple procedures

        // 2 procedures only
        // proc 2 + 3 -> non-nested + singly-nested
        shared_ptr<ProgramNode> programNode7 = make_shared<ProgramNode>(ProgramNode(
                {procedureNode2, procedureNode3}));

        // proc 3 + 4 -> singly-nested + doubly-nested
        shared_ptr<ProgramNode> programNode8 = make_shared<ProgramNode>(ProgramNode(
                {procedureNode3, procedureNode4}));

        // proc 4 + 5 -> doubly-nested + doubly-nested
        shared_ptr<ProgramNode> programNode9 = make_shared<ProgramNode>(ProgramNode(
                {procedureNode4, procedureNode5}));

        // 3 procedures only
        // proc 2 + 3 + 4 -> non-nested + singly-nested + doubly-nested
        shared_ptr<ProgramNode> programNode10 = make_shared<ProgramNode>(ProgramNode(
                {procedureNode2, procedureNode3, procedureNode4}));

        vector<shared_ptr<ProgramNode>> programNodes = vector<shared_ptr<ProgramNode>>{programNode1, programNode2,
                                                                                       programNode3, programNode4,
                                                                                       programNode5, programNode6,
                                                                                       programNode7, programNode8,
                                                                                       programNode9, programNode10};

        return programNodes[idx];
    }

} // TestDE