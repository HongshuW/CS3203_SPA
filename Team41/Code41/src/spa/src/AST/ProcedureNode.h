//
// Created by Tan Wei En on 2/9/22.
//

#include "ASTNode.h"
#include "AssignNode.h"
#include "StatementListNode.h"

#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

using namespace std;

class ProcedureNode : public ASTNode {
private:
    string procedureName;
    // will change to statement list in the future
    shared_ptr<StatementListNode> statementListNode;
public:
    ProcedureNode(string procedureName, shared_ptr<StatementListNode> statementListNode);
    StatementListNode getStatementListNode();
};

#endif //SPA_PROCEDURENODE_H
