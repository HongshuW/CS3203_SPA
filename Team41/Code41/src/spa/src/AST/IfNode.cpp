//
// Created by Xingchen Lin on 8/9/22.
//

#include "IfNode.h"

AST::IfNode::IfNode(shared_ptr<CondExprNode> condExpr, vector<shared_ptr<StmtNode>> ifStmtList, vector<shared_ptr<StmtNode>> elseStmtList) :
        condExpr(condExpr), ifStmtList(ifStmtList), elseStmtList(elseStmtList) {
    this->setNodeType("if");
};