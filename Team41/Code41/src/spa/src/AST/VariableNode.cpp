//
// Created by Tan Wei En on 2/9/22.
//

#include "VariableNode.h"

#include <utility>

using namespace AST;

VariableNode::VariableNode(string variable) : variable(std::move(variable)) {}

bool VariableNode::operator==(const ASTNode &node) const {
  auto castedNode = dynamic_cast<const VariableNode *>(&node);
  return castedNode != nullptr && variable == castedNode->variable;
}
