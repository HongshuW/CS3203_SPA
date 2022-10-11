//
// Created by Tan Wei En on 4/10/22.
//

#include "InvalidProcedureDummies.h"

namespace TestAST {

    shared_ptr<ProgramNode> InvalidProcedureDummies::getTestProgramNode(int idx) {

        /* procedure procedure1 {
         * print x1;
         * read variable;
         * call procedure2
         * }
         */

        auto calls_p1 = make_shared<CallNode>("procedure2");
        auto variable_p1 = make_shared<VariableNode>("x1");
        auto print_p1 = make_shared<PrintNode>(variable_p1);
        auto variable2_p1 = make_shared<VariableNode>("variable");
        auto read_p1 = make_shared<ReadNode>(variable2_p1);
        vector<shared_ptr<StmtNode>> statementList_p1 = vector<shared_ptr<StmtNode>>({calls_p1, print_p1, read_p1});
        auto procedure1_p1 = make_shared<ProcedureNode>("procedure1", statementList_p1);
        auto program1 = make_shared<ProgramNode>(vector<shared_ptr<ProcedureNode>>({procedure1_p1}));


        /* procedure procedure2 {
         * call procedure2
         * }
         */


        auto calls_p2 = make_shared<CallNode>("procedure2");
        vector<shared_ptr<StmtNode>> statementList_p2 = vector<shared_ptr<StmtNode>>({calls_p2});
        auto procedure1_p2 = make_shared<ProcedureNode>("procedure2", statementList_p2);
        auto program2 = make_shared<ProgramNode>(vector<shared_ptr<ProcedureNode>>({procedure1_p2}));

        /*
       * procedure3 {
       * 1 print x
       * 2 read y
       * 3 if (bar == y + 1123) then {
       * 4     call procedure3;
       * 5     print z
       *   } else {
       * 6     read w
       *   }
       * 7 read foo
       * }
       */

        shared_ptr<StmtNode> printNode_p3 = make_shared<PrintNode>(make_shared<VariableNode>("x"));
        shared_ptr<StmtNode> readNode_p3 = make_shared<ReadNode>(make_shared<VariableNode>("y"));
        //"bar == y + 1123"
        auto rhs_p3 = make_shared<ExprNode>("+");
        rhs_p3->left = make_shared<ExprNode>("y");
        rhs_p3->right = make_shared<ExprNode>("1123");
        shared_ptr<CondExprNode> ifCondExpr_p3 = make_shared<CondExprNode>(make_shared<RelExprNode>(make_shared<ExprNode>("bar"), "==",
                                                                                                    rhs_p3));

        shared_ptr<StmtNode> printNode2_p3 = make_shared<PrintNode>(make_shared<VariableNode>("z"));
        shared_ptr<StmtNode> readNode2_p3 = make_shared<ReadNode>(make_shared<VariableNode>("w"));
        shared_ptr<ExprNode> exprNode_3 = make_shared<ExprNode>("+");
        exprNode_3->left = make_shared<ExprNode>("y");
        exprNode_3->right = make_shared<ExprNode>("1");

        shared_ptr<CallNode> call_p3 = make_shared<CallNode>("procedure3");
        vector<shared_ptr<StmtNode>> ifStmtList_p3 = {call_p3, printNode2_p3};
        vector<shared_ptr<StmtNode>> elseStmtList_p3 = {readNode2_p3};
        shared_ptr<StmtNode> ifNode_p3 = make_shared<IfNode>(ifCondExpr_p3, ifStmtList_p3, elseStmtList_p3);

        shared_ptr<StmtNode> readNode3_p3 = make_shared<ReadNode>(make_shared<VariableNode>("foo"));

        shared_ptr<ProcedureNode> procedure_3 = make_shared<ProcedureNode>(ProcedureNode("procedure3", {printNode_p3, readNode_p3, ifNode_p3, readNode3_p3}));
        shared_ptr<ProgramNode> program3 = make_shared<ProgramNode>(ProgramNode({procedure_3}));

        /* procedure procedure1 {
         * read x;
         * read y;
         * if (x < 30) then {
         * call procedure2;
         * else {
         * print x;
         * }
         * }
         *
         * procedure procedure2 {
         * while (x < y) {
         * read z;
         * call procedure3;
         * call procedure4
         * }
         * }
         *
         * procedure procedure3 {
         * z = x + y;
         * }
         *
         * procedure procedure4 {
         * call procedure1;
         * }
         */

        auto read_p4 = make_shared<ReadNode>(make_shared<VariableNode>("x"));
        auto read2_p4 = make_shared<ReadNode>(make_shared<VariableNode>("y"));
        vector<shared_ptr<StmtNode>> statementList_p4 = vector<shared_ptr<StmtNode>>({make_shared<CallNode>("procedure2")});
        vector<shared_ptr<StmtNode>> elseStatementList_p4 = vector<shared_ptr<StmtNode>>({make_shared<PrintNode>(
                make_shared<VariableNode>("z"))});

        auto rhs_p4 = make_shared<ExprNode>("+");
        rhs_p4->left = make_shared<ExprNode>("y");
        rhs_p4->right = make_shared<ExprNode>("1123");
        shared_ptr<CondExprNode> ifCondExpr_p4 = make_shared<CondExprNode>(make_shared<RelExprNode>(make_shared<ExprNode>("bar"), "==",
                                                                                                    rhs_p4));
        auto if_p4 = make_shared<IfNode>(ifCondExpr_p4, statementList_p4, elseStatementList_p4);
        vector<shared_ptr<StmtNode>> procedure1_stmtList = vector<shared_ptr<StmtNode>>({read_p4, read2_p4, if_p4});
        auto procedure1_p4 = make_shared<ProcedureNode>("procedure1",procedure1_stmtList);

        auto read3_p4 = make_shared<ReadNode>(make_shared<VariableNode>("z"));
        auto call2_p4 = make_shared<CallNode>("procedure3");
        auto call3_p4 = make_shared<CallNode>("procedure4");
        auto while_p4 = make_shared<WhileNode>(ifCondExpr_p4, vector<shared_ptr<StmtNode>>({read3_p4, call2_p4, call3_p4}));
        vector<shared_ptr<StmtNode>> procedure2_stmtList = vector<shared_ptr<StmtNode>>({while_p4});
        auto procedure2_p4 = make_shared<ProcedureNode>("procedure2", procedure2_stmtList);

        shared_ptr<ExprNode> exprNode_p4 = make_shared<ExprNode>("+");
        exprNode_p4->left = make_shared<ExprNode>("y");
        exprNode_p4->right = make_shared<ExprNode>("1");
        shared_ptr<StmtNode> assignNode_p4 = make_shared<AssignNode>(make_shared<VariableNode>("x"), exprNode_p4);
        vector<shared_ptr<StmtNode>> procedure3_stmtList = vector<shared_ptr<StmtNode>>({assignNode_p4});
        auto procedure3_p4 = make_shared<ProcedureNode>("procedure3", procedure3_stmtList);

        auto call4_p4 = make_shared<CallNode>("procedure1");
        vector<shared_ptr<StmtNode>> procedure4_stmtList = vector<shared_ptr<StmtNode>>({call4_p4});
        auto procedure4_p4 = make_shared<ProcedureNode>("procedure4", procedure4_stmtList);

        vector<shared_ptr<ProcedureNode>> program4_stmtList = vector<shared_ptr<ProcedureNode>>({procedure1_p4, procedure2_p4, procedure3_p4, procedure4_p4});
        auto program4 = make_shared<ProgramNode>(program4_stmtList);

        /*procedure procedure1 {
         * call procedure2;
         * }
         *
         * procedure procedure2 {
         * call procedure1;
         * }
         */

        auto call_p5 = make_shared<CallNode>("procedure2");
        vector<shared_ptr<StmtNode>> stmtList_p5 = vector<shared_ptr<StmtNode>>({call_p5});
        auto procedure1_p5 = make_shared<ProcedureNode>("procedure1", stmtList_p5);

        auto call2_p5 = make_shared<CallNode>("procedure1");
        vector<shared_ptr<StmtNode>> stmtList2_p5 = vector<shared_ptr<StmtNode>>({call2_p5});
        auto procedure2_p5 = make_shared<ProcedureNode>("procedure2", stmtList2_p5);

        vector<shared_ptr<ProcedureNode>> program5_stmtList = vector<shared_ptr<ProcedureNode>>({procedure1_p5, procedure2_p5});
        auto program5 = make_shared<ProgramNode>(program5_stmtList);


        /*procedure procedure1 {
         * call procedure2;
         * }
         *
         * procedure procedurE2 {
         * read x;
         * }
         */

        auto call_p6 = make_shared<CallNode>("procedure2");
        vector<shared_ptr<StmtNode>> stmtList_p6 = vector<shared_ptr<StmtNode>>({call_p6});
        auto procedure1_p6 = make_shared<ProcedureNode>("procedure1", stmtList_p6);

        auto read_p6 = make_shared<ReadNode>(make_shared<VariableNode>("x"));
        vector<shared_ptr<StmtNode>> stmtList2_p6 = vector<shared_ptr<StmtNode>>({read_p6});
        auto procedure2_p6 = make_shared<ProcedureNode>("procedurE2", stmtList2_p6);

        vector<shared_ptr<ProcedureNode>> program6_stmtList = vector<shared_ptr<ProcedureNode>>({procedure1_p6, procedure2_p6});
        auto program6 = make_shared<ProgramNode>(program6_stmtList);

        /*procedure duplicateProcedure {
         * read y;
         * }
         *
         * procedure duplicateProcedure {
         * read x;
         * }
         */

        auto x_p7 = make_shared<VariableNode>("x");
        auto read_p7 = make_shared<ReadNode>(x_p7);
        vector<shared_ptr<StmtNode>> stmtList1_p7 = vector<shared_ptr<StmtNode>>({read_p7});
        auto procedure1_p7 = make_shared<ProcedureNode>("duplicateProcedure", stmtList1_p7);

        auto x2_p7 = make_shared<VariableNode>("y");
        auto read2_p7 = make_shared<ReadNode>(x2_p7);
        vector<shared_ptr<StmtNode>> stmtList2_p7 = vector<shared_ptr<StmtNode>>({read2_p7});
        auto procedure2_p7 = make_shared<ProcedureNode>("duplicateProcedure", stmtList2_p7);

        vector<shared_ptr<ProcedureNode>> program7_stmtList = vector<shared_ptr<ProcedureNode>>({procedure1_p7, procedure2_p7});
        auto program7 = make_shared<ProgramNode>(program7_stmtList);

        vector<shared_ptr<ProgramNode>> programNodes = vector<shared_ptr<ProgramNode>>{program1, program2,
                                                                                       program3, program4,
                                                                                       program5, program6,
                                                                                       program7
                                                                                       };

        return programNodes[idx];
    }

} // TestDE