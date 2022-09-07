//
// Created by Tan Wei En on 2/9/22.
//
//
#include "ASTNode.h"
//

string ASTNode::getNodeType() {
    return nodeType;
}

void ASTNode::setNodeType(std::string type) {
    nodeType = move(type);
}