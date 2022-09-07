//
// Created by Tan Wei En on 2/9/22.
//

#include "NumberNode.h"
#include "iostream"

NumberNode::NumberNode(string number) : number(number) {
    this -> setNodeType("number");
}

string  NumberNode::getNumber() {
    return number;
}