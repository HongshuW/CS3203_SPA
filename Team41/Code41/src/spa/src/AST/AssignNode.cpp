//
// Created by Tan Wei En on 3/9/22.
//

#include "AssignNode.h"

AST::AssignNode::AssignNode(shared_ptr<VariableNode> variableNode, shared_ptr<ExprNode> expressionNode) :
    variableNode(variableNode),
    expressionNode(expressionNode) {}

bool AssignNode::operator==(const ASTNode &node) const {
    auto castedNode = dynamic_cast<const AssignNode*>(&node);
    return castedNode != nullptr && *variableNode == *castedNode->variableNode
           && *expressionNode == *castedNode->expressionNode;
};
