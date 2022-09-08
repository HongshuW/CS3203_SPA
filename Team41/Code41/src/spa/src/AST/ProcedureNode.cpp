//
// Created by Tan Wei En on 2/9/22.
//

#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(string procedureName, shared_ptr<StatementListNode> statementListNode) : procedureName(procedureName), statementListNode(statementListNode) {
    this -> setNodeType("procedure");
}

StatementListNode ProcedureNode::getStatementListNode() {
    return *statementListNode;
}