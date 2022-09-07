//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H

#include "ASTNode.h"
#include "VariableNode.h"

using namespace std;

class PrintNode : public ASTNode {
private:
    shared_ptr<VariableNode> variableNode;

public:
    PrintNode(shared_ptr<VariableNode> variableNode);
    VariableNode getVariableNode();
};

#endif //SPA_PRINTNODE_H
