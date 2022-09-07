//
// Created by Tan Wei En on 2/9/22.
//

#include "VariableNode.h"
#include "iostream"

VariableNode::VariableNode(string variable)
        : variable(variable) {
    this -> setNodeType("variable");
}

string VariableNode::getVariable() {
    return variable;
}