//
// Created by Xingchen Lin on 8/9/22.
//

#include "CallNode.h"

using namespace AST;

AST::CallNode::CallNode(shared_ptr<VariableNode> variableNode) : variableNode(variableNode) {
    this ->setNodeType("call");
}