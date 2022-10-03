//
// Created by Tan Wei En on 3/10/22.
//

#include "catch.hpp"

#include "parser/Parser.h"
#include "parser/Tokenizer.h"
#include "parser/SPExceptions.h"
#include "parser/ASTValidator.h"
#include "iostream"
#include "unordered_map"

using namespace std;
using namespace SourceParser;

TEST_CASE("Test AST Validator") {
    SECTION("Test valid AST") {
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
        shared_ptr<CondExprNode> ifCondExpr_p14 = make_shared<CondExprNode>("x = 0");

        vector<shared_ptr<StmtNode>> ifStmtLst_p14{callNode_14};
        vector<shared_ptr<StmtNode>> elseStmtLst_p14{callNode_14_1};
        shared_ptr<IfNode> ifNode_p14 = make_shared<IfNode>(ifCondExpr_p14,
                                                            ifStmtLst_p14, elseStmtLst_p14);
        shared_ptr<CondExprNode> a_equal_three = make_shared<CondExprNode>(make_shared<RelExprNode>(
                make_shared<ExprNode>("a"), "==", make_shared<ExprNode>("3")));
        vector<shared_ptr<StmtNode>> whileStmtLst_p14 {ifNode_p14};
        shared_ptr<WhileNode> whileNode_p14 = make_shared<WhileNode>(a_equal_three,
                                                                     whileStmtLst_p14);

        shared_ptr<ProcedureNode> procedureNode14 = make_shared<ProcedureNode>(ProcedureNode("procedure2",{
                printNode_14, whileNode_p14, assignNode_14
        }));

        shared_ptr<StmtNode> printNode_14_1 = make_shared<PrintNode>(make_shared<VariableNode>("y"));
        shared_ptr<StmtNode> callNode_14_1_1 = make_shared<CallNode>("procedure5");
        shared_ptr<StmtNode> readNode_14_1 = make_shared<ReadNode>(make_shared<VariableNode>("oloha"));

        vector<shared_ptr<StmtNode>> whileStmtLst_p14_1 {callNode_14_1_1, readNode_14_1};
        shared_ptr<CondExprNode> b_equal_four = make_shared<CondExprNode>(make_shared<RelExprNode>(
                make_shared<ExprNode>("b"), "==", make_shared<ExprNode>("4")));
        shared_ptr<WhileNode> whileNode_p14_1 = make_shared<WhileNode>(make_shared<CondExprNode>(b_equal_four),
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

        ASTValidator astValidator = ASTValidator(programNode14);
        astValidator.validateAST();
    }
}