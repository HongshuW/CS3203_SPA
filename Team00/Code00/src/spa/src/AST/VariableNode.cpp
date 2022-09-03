//
// Created by Tan Wei En on 2/9/22.
//

#include "VariableNode.h"
#include "iostream"

VariableNode::VariableNode(std::string variable)
    : variable(variable) {
    std::cout << variable << std::endl;
}