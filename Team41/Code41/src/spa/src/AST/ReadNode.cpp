//
// Created by Tan Wei En on 8/9/22.
//

#include "ReadNode.h"

#include <utility>

using namespace AST;

AST::ReadNode::ReadNode(shared_ptr<VariableNode> variableNode)
    : variableNode(std::move(variableNode)) {}

bool ReadNode::operator==(const ASTNode &node) const {
  auto castedNode = dynamic_cast<const ReadNode *>(&node);
  return castedNode != nullptr && *variableNode == *castedNode->variableNode;
}