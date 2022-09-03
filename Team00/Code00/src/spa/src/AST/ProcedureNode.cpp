//
// Created by Tan Wei En on 2/9/22.
//

#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(std::string procedureName, std::shared_ptr<AssignNode> assignNode)
    : procedureName(procedureName), assignNode(assignNode) {}