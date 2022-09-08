//
// Created by Tan Wei En on 8/9/22.
//

#include "ReadNode.h"

using namespace AST;

AST::ReadNode::ReadNode(shared_ptr<VariableNode> variableNode) : variableNode(variableNode) {};