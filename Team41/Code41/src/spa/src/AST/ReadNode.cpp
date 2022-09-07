//
// Created by Tan Wei En on 8/9/22.
//

#include "ReadNode.h"

ReadNode::ReadNode(shared_ptr<VariableNode> variableNode) : variableNode(variableNode) {
    this -> setNodeType("read");
};


VariableNode ReadNode::getVariableNode() {
    return *variableNode;
}