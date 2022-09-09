//
// Created by Xingchen Lin on 8/9/22.
//

#include "WhileNode.h"

AST::WhileNode::WhileNode(shared_ptr<CondExprNode> condExpr, vector<shared_ptr<StmtNode>> stmtList) :
    condExpr(condExpr), stmtList(stmtList) {};
