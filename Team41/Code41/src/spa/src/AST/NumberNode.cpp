//
// Created by Tan Wei En on 2/9/22.
//

#include "NumberNode.h"
#include "iostream"

NumberNode::NumberNode(std::string number)
        : number(number) {
}

std::string  NumberNode::getNumber() {
    return number;
}