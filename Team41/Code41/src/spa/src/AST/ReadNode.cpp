//
// Created by Tan Wei En on 8/9/22.
//

#include "ReadNode.h"

using namespace AST;

AST::ReadNode::ReadNode(shared_ptr<VariableNode> variableNode) : variableNode(variableNode) {}

bool ReadNode::operator==(const ASTNode &node) const {
    auto castedNode = dynamic_cast<const ReadNode*>(&node);
    return castedNode != 0 && *variableNode == *castedNode->variableNode;
};