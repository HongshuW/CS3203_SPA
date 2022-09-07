//
// Created by Tan Wei En on 2/9/22.
//

#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(string procedureName, shared_ptr<AssignNode> assignNode) : procedureName(procedureName), assignNode(assignNode) {
    this -> setNodeType("procedure");
}

AssignNode ProcedureNode::getAssignNode() {
    return *assignNode;
}