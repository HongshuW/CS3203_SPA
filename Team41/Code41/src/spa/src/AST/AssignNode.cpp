//
// Created by Tan Wei En on 3/9/22.
//

#include "AssignNode.h"

AssignNode::AssignNode(shared_ptr<VariableNode> variableNode, shared_ptr<ExpressionNode> expressionNode) : variableNode(variableNode), expressionNode(expressionNode) {
    this -> setNodeType("assign");
};


VariableNode AssignNode::getVariableNode() {
    return *variableNode;
}

ExpressionNode AssignNode::getExpressionNode() {
    return *expressionNode;
}