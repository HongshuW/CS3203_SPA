//
// Created by Xingchen Lin on 10/9/22.
//

#include "RelExprNode.h"

RelExprNode::RelExprNode(shared_ptr<ExprNode> exprNodeLHS, string op, shared_ptr<ExprNode> exprNodeRHS) :
        exprNodeLHS(exprNodeLHS), op(op), exprNodeRHS(exprNodeRHS){}

bool RelExprNode::operator==(const ASTNode &node) const {
    auto castedNode = dynamic_cast<const RelExprNode*>(&node);

    return castedNode != nullptr
           && (exprNodeLHS == castedNode->exprNodeLHS || *exprNodeLHS == *castedNode->exprNodeLHS)
           && (exprNodeRHS == castedNode->exprNodeRHS || *exprNodeRHS == *castedNode->exprNodeRHS)
           && op == castedNode->op;
};