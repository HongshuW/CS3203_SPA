//
// Created by Tan Wei En on 3/9/22.
//

#include "AssignNode.h"

AssignNode::AssignNode(shared_ptr<VariableNode> variableNode, shared_ptr<NumberNode> numberNode)
    : variableNode(variableNode), numberNode(numberNode) {};
