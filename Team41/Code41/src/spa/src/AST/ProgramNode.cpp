//
// Created by Tan Wei En on 3/9/22.
//

#include "ProgramNode.h"

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedureList)
        : procedureList(procedureList) {}

vector<shared_ptr<ProcedureNode>> ProgramNode::getProcedureList() {
    return procedureList;
}