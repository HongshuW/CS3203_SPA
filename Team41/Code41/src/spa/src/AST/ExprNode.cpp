//
// Created by Xingchen Lin on 8/9/22.
//

#include "ExprNode.h"

#include <utility>

AST::ExprNode::ExprNode(string expr) : expr(std::move(expr)) {}

bool ExprNode::isNumberNode() const {
    return Utils::isValidNumber(expr);
}

bool ExprNode::isVariableNode() const {
    return Utils::isValidName(expr);
}

bool ExprNode::isOperatorNode() const {
    return Utils::isMathOperators(expr);
}

bool ExprNode::operator==(const ASTNode &node) const{
    auto castedNode = dynamic_cast<const ExprNode*>(&node);

    return castedNode != nullptr
           && (left == castedNode->left || *left == *castedNode->left)
           && (right == castedNode->right || *right == *castedNode->right)
           && expr == castedNode->expr;
}
