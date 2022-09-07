//
// Created by Tan Wei En on 3/9/22.
//

#include "AssignNode.h"
#include "iostream"

AssignNode::AssignNode(shared_ptr<VariableNode> variableNode, shared_ptr<NumberNode> numberNode)
        : variableNode(variableNode), numberNode(numberNode) {
    this ->setNodeType("assign");
};


VariableNode AssignNode::getVariableNode() {
    return *variableNode;
}

NumberNode AssignNode::getNumberNode() {
    return *numberNode;
}