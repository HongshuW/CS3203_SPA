//
// Created by Xingchen Lin on 8/9/22.
//

#include "CondExprNode.h"

using namespace AST;

CondExprNode::CondExprNode(string condExpr) : condExpr(condExpr) {};

CondExprNode::CondExprNode(shared_ptr<RelExprNode> relExprNode) : relExprNode(relExprNode) {};

CondExprNode::CondExprNode(shared_ptr<CondExprNode> condExprLHS) : condExprLHS(condExprLHS) {};

CondExprNode::CondExprNode(shared_ptr<CondExprNode> condExprLHS, string op, shared_ptr<CondExprNode> condExprRHS) :
condExprLHS(condExprLHS), op(op), condExprRHS(condExprRHS) {}

bool CondExprNode::operator==(const ASTNode &node) const  {
    auto castedNode = dynamic_cast<const CondExprNode*>(&node);

    return castedNode != nullptr
           && (relExprNode == castedNode->relExprNode || *relExprNode == *castedNode->relExprNode)
           && (condExprLHS == castedNode->condExprLHS || *condExprLHS == *castedNode->condExprLHS)
           && (condExprRHS == castedNode->condExprRHS || *condExprRHS == *castedNode->condExprRHS)
           && op == castedNode->op
           && condExpr == castedNode->condExpr;
};

