//
// Created by Xingchen Lin on 10/9/22.
//

#include "RelExprNode.h"

RelExprNode::RelExprNode(shared_ptr<ExprNode> exprNodeLHS, string op, shared_ptr<ExprNode> exprNodeRHS) :
        exprNodeLHS(exprNodeLHS), op(op), exprNodeRHS(exprNodeRHS){};