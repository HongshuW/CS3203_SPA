//
// Created by Xingchen Lin on 8/9/22.
//

#include "ExprNode.h"

AST::ExprNode::ExprNode(string expr) : expr(expr) {}

bool ExprNode::operator==(const ASTNode &node) const{
    auto castedNode = dynamic_cast<const ExprNode*>(&node);

    return castedNode != 0
           && (left == castedNode->left || *left == *castedNode->left)
           && (right == castedNode->right || *right == *castedNode->right)
           && expr == castedNode->expr;
}
