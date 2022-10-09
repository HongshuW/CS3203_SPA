//
// Created by Xingchen Lin on 8/9/22.
//

#include "CallNode.h"

#include <utility>

using namespace AST;

AST::CallNode::CallNode(string programName) : procedureName(std::move(programName)) {}

bool AST::CallNode::operator==(const ASTNode& node) const {
    auto castedNode = dynamic_cast<const CallNode*>(&node);
    return castedNode != nullptr && procedureName == castedNode->procedureName;
}