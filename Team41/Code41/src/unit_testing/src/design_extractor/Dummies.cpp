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
       * 7 read foo
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

        //procedures for call extraction testing

        // 11
        /*
       * procedure2 {
       * 1 print x
       * 2 call procedure3
       * 3 x = y + 1
       * }
       * procedure3 {
       * 4 print dah
       * 5 read z
       * }
       */
        shared_ptr<StmtNode> printNode_11 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> callNode_11 = make_shared<CallNode>("procedure3");
        shared_ptr<ExprNode> exprNode_11 = make_shared<ExprNode>("+");
        exprNode_11->left = make_shared<ExprNode>("y");
        exprNode_11->right = make_shared<ExprNode>("1");
        shared_ptr<StmtNode> assignNode_11 = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                exprNode_11);
        shared_ptr<ProcedureNode> procedureNode11 = make_shared<ProcedureNode>(ProcedureNode("procedure2",
                                                                                             {printNode_11,
                                                                                              callNode_11,
                                                                                              assignNode_11}));

        shared_ptr<StmtNode> printNode_11_1 = make_shared<PrintNode>(make_shared<VariableNode>("dah"));
        shared_ptr<ReadNode> readNode_11_1 = make_shared<ReadNode>(make_shared<VariableNode>("z"));
        shared_ptr<ProcedureNode> procedureNode11_1 = make_shared<ProcedureNode>(ProcedureNode("procedure3",
                                                                                               {printNode_11_1,
                                                                                                readNode_11_1}));
        shared_ptr<ProgramNode> programNode11 = make_shared<ProgramNode>(ProgramNode({procedureNode11,
                                                                                      procedureNode11_1}));

        // 12
        /*
       * procedure2 {
       * 1 print x
       * 2 call procedure3
       * 3 call procedure4
       * 4 x = y + 1
       * }
       * procedure3 {
       * 5 print y
       * 6 read adda
       * 7 call procedure4
       * }
       * procedure4 {
       * 8 print z
       * 9 c = a + b
       * }
       */

        shared_ptr<StmtNode> printNode_12 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> callNode_12 = make_shared<CallNode>("procedure3");
        shared_ptr<StmtNode> callNode_12_1 = make_shared<CallNode>("procedure4");
        shared_ptr<ExprNode> exprNode_12 = make_shared<ExprNode>("+");
        exprNode_12->left = make_shared<ExprNode>("y");
        exprNode_12->right = make_shared<ExprNode>("1");
        shared_ptr<StmtNode> assignNode_12 = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                                                                     exprNode_12);
        shared_ptr<ProcedureNode> procedureNode12 = make_shared<ProcedureNode>(ProcedureNode("procedure2",
                                                                                             {printNode_11,
                                                                                              callNode_12,
                                                                                              callNode_12_1,
                                                                                              assignNode_12}));

        shared_ptr<StmtNode> printNode_12_1 = make_shared<PrintNode>(make_shared<VariableNode>("y"));
        shared_ptr<StmtNode> readNode_12_1 = make_shared<ReadNode>(make_shared<VariableNode>("adda"));

        shared_ptr<StmtNode> callNode_12_1_1 = make_shared<CallNode>("procedure4");
        shared_ptr<ProcedureNode> procedureNode12_1 = make_shared<ProcedureNode>(ProcedureNode("procedure3",
                                                                                            {printNode_12_1,
                                                                                             readNode_12_1,
                                                                                             callNode_12_1_1}));

        shared_ptr<ExprNode> exprNode_12_2 = make_shared<ExprNode>("+");
        exprNode_12_2 -> left = make_shared<ExprNode>("a");
        exprNode_12_2 -> right = make_shared<ExprNode>("b");
        shared_ptr<StmtNode> assignNode_12_2 = make_shared<AssignNode>(make_shared<VariableNode>("c"),
                                                                     exprNode_12_2);
        shared_ptr<StmtNode> printNode_12_2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<ProcedureNode> procedureNode12_2 = make_shared<ProcedureNode>(ProcedureNode("procedure4",
                                                                                               {printNode_12_2,
                                                                                                assignNode_12_2}));

        shared_ptr<ProgramNode> programNode12 = make_shared<ProgramNode>(ProgramNode({procedureNode12,
                                                                                      procedureNode12_1,
                                                                                      procedureNode12_2}));

        // 13
        /*
       * procedure2 {
       * 1 print x
       * 2 if (x == 0) then {
       * 3   call procedure3
       * 4   print qwerty
       *   } else {
       * 5   call procedure4
       *   }
       * 6 x = y + 1
       * }
       * procedure3 {
       * 7 print y
       * 8 call procedure4
       * 9 call procedure6
       * }
       * procedure4 {
       * 10 print z
       * }
       * procedure6 {
       * 11 abc = xyz + def
       * }
       */

        shared_ptr<StmtNode> printNode_13 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<ExprNode> exprNode_13 = make_shared<ExprNode>("+");
        exprNode_13->left = make_shared<ExprNode>("y");
        exprNode_13->right = make_shared<ExprNode>("1");
        shared_ptr<StmtNode> assignNode_13 = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                                                                     exprNode_13);

        shared_ptr<StmtNode> callNode_13 = make_shared<CallNode>("procedure3");
        shared_ptr<StmtNode> callNode_13_1 = make_shared<CallNode>("procedure4");
        shared_ptr<CondExprNode> ifCondExpr_p13 = make_shared<CondExprNode>("x == 0");
        shared_ptr<StmtNode> printNode_13_01 = make_shared<PrintNode>(make_shared<VariableNode>("qwerty"));

        vector<shared_ptr<StmtNode>> ifStmtLst_p13{callNode_13, printNode_13_01};
        vector<shared_ptr<StmtNode>> elseStmtLst_p13{callNode_13_1};
        shared_ptr<IfNode> ifNode_p13 = make_shared<IfNode>(ifCondExpr_p13,
                ifStmtLst_p13, elseStmtLst_p13);

        shared_ptr<ProcedureNode> procedureNode13 = make_shared<ProcedureNode>(ProcedureNode("procedure2",
                                                                                             {printNode_13,
                                                                                              ifNode_p13,
                                                                                              assignNode_13}));

        shared_ptr<StmtNode> printNode_13_1 = make_shared<PrintNode>(make_shared<VariableNode>("y"));
        shared_ptr<StmtNode> callNode_13_1_1 = make_shared<CallNode>("procedure4");
        shared_ptr<StmtNode> callNode_13_1_2 = make_shared<CallNode>("procedure6");
        shared_ptr<ProcedureNode> procedureNode13_1 = make_shared<ProcedureNode>(ProcedureNode("procedure3",
                                                                                               {printNode_13_1,
                                                                                                callNode_13_1_1,
                                                                                                callNode_13_1_2}));

        shared_ptr<StmtNode> printNode_13_2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<ProcedureNode> procedureNode13_2 = make_shared<ProcedureNode>(ProcedureNode("procedure4",
                                                                                               {printNode_13_2}));


        shared_ptr<ExprNode> exprNode_13_3 = make_shared<ExprNode>("+");
        exprNode_13_3->left = make_shared<ExprNode>("xyz");
        exprNode_13_3->right = make_shared<ExprNode>("def");
        shared_ptr<StmtNode> assignNode_13_3 = make_shared<AssignNode>(make_shared<VariableNode>("abc"),
                                                                     exprNode_13_3);

        shared_ptr<ProcedureNode> procedureNode13_3 = make_shared<ProcedureNode>(ProcedureNode("procedure6",
                                                                                               {assignNode_13_3}));




        shared_ptr<ProgramNode> programNode13 = make_shared<ProgramNode>(ProgramNode({procedureNode13,
                                                                                      procedureNode13_1,
                                                                                      procedureNode13_2,
                                                                                      procedureNode13_3}));
        // 14
        /*
       * procedure2 {
       * 1 print x
       * 2 while(a == 3) {
       * 3   if (x == 0) then {
       * 4     call procedure3
       *     } else {
       * 5     call procedure4
       *     }
       *   }
       * 6 x = y + 1
       * }
       * procedure3 {
       * 7 print y
       * 8 while(b == 4) {
       * 9  call procedure5
       * 10 read oloha
       *   }
       * }
       * procedure4 {
       * 11 print z
       * 12 call procedure6
       * 13 read blabla
       * }
       * procedure5 {
       * 14 print asdf
       * }
       * procedure6 {
       * 15 zxcv = vvv + xxx
       * }
       */

        shared_ptr<StmtNode> printNode_14 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<ExprNode> exprNode_14 = make_shared<ExprNode>("+");
        exprNode_14->left = make_shared<ExprNode>("y");
        exprNode_14->right = make_shared<ExprNode>("1");
        shared_ptr<StmtNode> assignNode_14 = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                                                                     exprNode_14);

        shared_ptr<StmtNode> callNode_14 = make_shared<CallNode>("procedure3");
        shared_ptr<StmtNode> callNode_14_1 = make_shared<CallNode>("procedure4");
        shared_ptr<CondExprNode> ifCondExpr_p14 = make_shared<CondExprNode>("x == 0");

        vector<shared_ptr<StmtNode>> ifStmtLst_p14{callNode_14};
        vector<shared_ptr<StmtNode>> elseStmtLst_p14{callNode_14_1};
        shared_ptr<IfNode> ifNode_p14 = make_shared<IfNode>(ifCondExpr_p14,
                                                            ifStmtLst_p14, elseStmtLst_p14);
        vector<shared_ptr<StmtNode>> whileStmtLst_p14 {ifNode_p14};
        shared_ptr<WhileNode> whileNode_p14 = make_shared<WhileNode>(make_shared<CondExprNode>("a == 3"),
                whileStmtLst_p14);

        shared_ptr<ProcedureNode> procedureNode14 = make_shared<ProcedureNode>(ProcedureNode("procedure2",{
            printNode_14, whileNode_p14, assignNode_14
        }));

        shared_ptr<StmtNode> printNode_14_1 = make_shared<PrintNode>(make_shared<VariableNode>("y"));
        shared_ptr<StmtNode> callNode_14_1_1 = make_shared<CallNode>("procedure5");
        shared_ptr<StmtNode> readNode_14_1 = make_shared<ReadNode>(make_shared<VariableNode>("oloha"));

        vector<shared_ptr<StmtNode>> whileStmtLst_p14_1 {callNode_14_1_1, readNode_14_1};
        shared_ptr<WhileNode> whileNode_p14_1 = make_shared<WhileNode>(make_shared<CondExprNode>("b == 4"),
                                                                     whileStmtLst_p14_1);
        shared_ptr<ProcedureNode> procedureNode14_1 = make_shared<ProcedureNode>(ProcedureNode("procedure3",
                                                                                               {printNode_14_1,
                                                                                                whileNode_p14_1}));

        shared_ptr<StmtNode> printNode_14_2 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> callNode_14_1_2 = make_shared<CallNode>("procedure6");
        shared_ptr<StmtNode> readNode_14_1_2 = make_shared<ReadNode>(make_shared<VariableNode>("blabla"));

        shared_ptr<ProcedureNode> procedureNode14_2 = make_shared<ProcedureNode>(ProcedureNode("procedure4",
                                                                                               {printNode_14_2,
                                                                                                callNode_14_1_2,
                                                                                                readNode_14_1_2}));

        shared_ptr<StmtNode> printNode_14_3 = make_shared<PrintNode>(make_shared<VariableNode>("asdf"));
        shared_ptr<ProcedureNode> procedureNode14_3 = make_shared<ProcedureNode>(ProcedureNode("procedure5",
                                                                                               {printNode_14_3}));

        shared_ptr<ExprNode> exprNode_14_4 = make_shared<ExprNode>("+");
        exprNode_14_4->left = make_shared<ExprNode>("vvv");
        exprNode_14_4->right = make_shared<ExprNode>("xxx");
        shared_ptr<StmtNode> assignNode_14_4 = make_shared<AssignNode>(make_shared<VariableNode>("zxcv"),
                                                                       exprNode_14_4);
        shared_ptr<ProcedureNode> procedureNode14_4 = make_shared<ProcedureNode>(ProcedureNode("procedure6",
                                                                                               {assignNode_14_4}));


        shared_ptr<ProgramNode> programNode14 = make_shared<ProgramNode>(ProgramNode({procedureNode14,
                                                                                      procedureNode14_1,
                                                                                      procedureNode14_2,
                                                                                      procedureNode14_3,
                                                                                      procedureNode14_4}));

        // 15
        /*
         * procedure7 {
         * 1 x = 0
         * 2 while (x < 5) {
         * 3   x = 10
         *   }
         * 4 y = x
         * }
         */
        // 1
        shared_ptr<AssignNode> assignNode1_p7 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("0"));
        // 3
        shared_ptr<AssignNode> assignNode2_p7 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("10"));
        // 4
        shared_ptr<AssignNode> assignNode3_p7 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("x"));
        // 2
        vector<shared_ptr<StmtNode>> whileStmtLst_p7{assignNode2_p7};
        shared_ptr<WhileNode> whileNode_p7 = make_shared<WhileNode>(make_shared<CondExprNode>("x < 5"), whileStmtLst_p7);
        // procedure & program
        shared_ptr<ProcedureNode> procedureNode7 = make_shared<ProcedureNode>(ProcedureNode("procedure7", {assignNode1_p7, whileNode_p7, assignNode3_p7}));
        shared_ptr<ProgramNode> programNode15 = make_shared<ProgramNode>(ProgramNode({procedureNode7}));




        // 16
        /*
         * procedure8 {
         * 1 if (x == 0) {
         * 2   if (y == 0) {
         * 3     x = 1
         *     } else {
         * 4     y = 0
         *     }
         *   } else {
         * 5   x = 0
         *   }
         * }
         */
        // 3
        shared_ptr<AssignNode> assignNode1_p8 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("1"));
        // 4
        shared_ptr<AssignNode> assignNode2_p8 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("0"));
        // 5
        shared_ptr<AssignNode> assignNode3_p8 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("0"));
        // 2
        vector<shared_ptr<StmtNode>> ifStmtLst1_p8{assignNode1_p8};
        vector<shared_ptr<StmtNode>> elseStmtLst1_p8{assignNode2_p8};
        shared_ptr<IfNode> ifNode1_p8 = make_shared<IfNode>(make_shared<CondExprNode>("y == 0"), ifStmtLst1_p8, elseStmtLst1_p8);
        // 1
        vector<shared_ptr<StmtNode>> ifStmtLst2_p8{ifNode1_p8};
        vector<shared_ptr<StmtNode>> elseStmtLst2_p8{assignNode3_p8};
        shared_ptr<IfNode> ifNode2_p8 = make_shared<IfNode>(make_shared<CondExprNode>("x == 0"), ifStmtLst2_p8, elseStmtLst2_p8);
        // procedure & program
        shared_ptr<ProcedureNode> procedureNode8 = make_shared<ProcedureNode>(ProcedureNode("procedure8", {ifNode2_p8}));
        shared_ptr<ProgramNode> programNode16 = make_shared<ProgramNode>(ProgramNode({procedureNode8}));



        // 17
        /*
         * procedure9 {
         * 1 x = 0
         * 2 while (x < 5) {
         * 3   if (x == 0) {
         * 4     x = 1
         *     } else {
         * 5     while (x != 0) {
         * 6       x = 2
         * 7       if (y == 0) {
         * 8         y = 1
         *         } else {
         * 9         y = 2
         *         }
         * 10      if (z == 0) {
         * 11        z = 1
         *         } else {
         * 12        x = 5
         *         }
         *       }
         *     }
         *   }
         * }
         */
        // 1
        shared_ptr<AssignNode> assignNode1_p9 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("0"));
        // 4
        shared_ptr<AssignNode> assignNode2_p9 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("1"));
        // 6
        shared_ptr<AssignNode> assignNode3_p9 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("2"));
        // 8
        shared_ptr<AssignNode> assignNode4_p9 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("1"));
        // 9
        shared_ptr<AssignNode> assignNode5_p9 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("2"));
        // 11
        shared_ptr<AssignNode> assignNode6_p9 = make_shared<AssignNode>(make_shared<VariableNode>("z"), make_shared<ExprNode>("1"));
        // 12
        shared_ptr<AssignNode> assignNode7_p9 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("5"));
        // 7
        vector<shared_ptr<StmtNode>> ifStmtLst1_p9{assignNode4_p9};
        vector<shared_ptr<StmtNode>> elseStmtLst1_p9{assignNode5_p9};
        shared_ptr<IfNode> ifNode1_p9 = make_shared<IfNode>(make_shared<CondExprNode>("y == 0"), ifStmtLst1_p9, elseStmtLst1_p9);
        // 10
        vector<shared_ptr<StmtNode>> ifStmtLst2_p9{assignNode6_p9};
        vector<shared_ptr<StmtNode>> elseStmtLst2_p9{assignNode7_p9};
        shared_ptr<IfNode> ifNode2_p9 = make_shared<IfNode>(make_shared<CondExprNode>("z == 0"), ifStmtLst2_p9, elseStmtLst2_p9);
        // 5
        vector<shared_ptr<StmtNode>> whileStmtLst1_p9{assignNode3_p9, ifNode1_p9, ifNode2_p9};
        shared_ptr<WhileNode> whileNode1_p9 = make_shared<WhileNode>(make_shared<CondExprNode>("x != 0"), whileStmtLst1_p9);
        // 3
        vector<shared_ptr<StmtNode>> ifStmtLst3_p9{assignNode2_p9};
        vector<shared_ptr<StmtNode>> elseStmtLst3_p9{whileNode1_p9};
        shared_ptr<IfNode> ifNode3_p9 = make_shared<IfNode>(make_shared<CondExprNode>("x == 0"), ifStmtLst3_p9, elseStmtLst3_p9);
        // 2
        vector<shared_ptr<StmtNode>> whileStmtLst2_p9{ifNode3_p9};
        shared_ptr<WhileNode> whileNode2_p9 = make_shared<WhileNode>(make_shared<CondExprNode>("x < 5"), whileStmtLst2_p9);
        // procedure & program
        shared_ptr<ProcedureNode> procedureNode9 = make_shared<ProcedureNode>(ProcedureNode("procedure9", {assignNode1_p9, whileNode2_p9}));
        shared_ptr<ProgramNode> programNode17 = make_shared<ProgramNode>(ProgramNode({procedureNode9}));

       
        vector<shared_ptr<ProgramNode>> programNodes = vector<shared_ptr<ProgramNode>>{programNode1, programNode2,
                                                                                       programNode3, programNode4,
                                                                                       programNode5, programNode6,
                                                                                       programNode7, programNode8,
                                                                                       programNode9, programNode10,
                                                                                       programNode11, programNode12,
                                                                                       programNode13, programNode14,
                                                                                       programNode15, programNode16,
                                                                                       programNode17};

        return programNodes[idx];
    }

} // TestDE