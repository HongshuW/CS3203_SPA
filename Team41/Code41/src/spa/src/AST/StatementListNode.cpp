//
// Created by Tan Wei En on 8/9/22.
//

#include "StatementListNode.h"

StatementListNode::StatementListNode(vector<shared_ptr<ASTNode>> statementList) : statementList(statementList) {
    this -> setNodeType("stmtLst");
}

vector<shared_ptr<ASTNode>> StatementListNode::getStatementList() {
    return statementList;
}
