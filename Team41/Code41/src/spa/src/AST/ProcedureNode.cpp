//
// Created by Tan Wei En on 2/9/22.
//

#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(string procedureName, vector<shared_ptr<StmtNode>> stmtList) :
procedureName(procedureName),
stmtList(stmtList) {
    this ->setNodeType("procedure");
}