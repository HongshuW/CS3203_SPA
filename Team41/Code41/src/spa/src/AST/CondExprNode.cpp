//
// Created by Xingchen Lin on 8/9/22.
//

#include "CondExprNode.h"

using namespace AST;

CondExprNode::CondExprNode(string condExpr) : condExpr(condExpr) {};

CondExprNode::CondExprNode(shared_ptr<RelExprNode> relExprNode) : relExprNode(relExprNode) {};

CondExprNode::CondExprNode(shared_ptr<CondExprNode> condExprLHS) : condExprLHS(condExprLHS) {};

CondExprNode::CondExprNode(shared_ptr<CondExprNode> condExprLHS, string op, shared_ptr<CondExprNode> condExprRHS) :
condExprLHS(condExprLHS), op(op), condExprRHS(condExprRHS) {};

