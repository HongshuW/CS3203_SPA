//
// Created by Tan Wei En on 3/9/22.
//

#include "ProgramNode.h"

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedureList) : procedureList(procedureList) {
    this->setNodeType("program")
}