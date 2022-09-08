//
// Created by Tan Wei En on 8/9/22.
//

#include "PrintNode.h"

using namespace AST;

AST::PrintNode::PrintNode(shared_ptr<VariableNode> variableNode) : variableNode(variableNode) {
    this ->setNodeType("print");
};
