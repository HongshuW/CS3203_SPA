//
// Created by Xingchen Lin on 8/9/22.
//

#include "CondExprNode.h"

using namespace AST;
CondExprNode::CondExprNode(std::string left, std::string op, std::string right) : left(left), op(op), right(right) {}