//
// Created by Tan Wei En on 2/9/22.
//

#include "ASTNode.h"
#include "AssignNode.h"

#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

using namespace std;

class ProcedureNode : public ASTNode {
private:
    string procedureName;
    // will change to statement list in the future
    shared_ptr<AssignNode> assignNode;
public:
    ProcedureNode(string procedureName, shared_ptr<AssignNode> assignNode);
    AssignNode getAssignNode();
};

#endif //SPA_PROCEDURENODE_H
