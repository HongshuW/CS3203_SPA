//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_READNODE_H
#define SPA_READNODE_H

#include "ASTNode.h"
#include "VariableNode.h"

using namespace std;

class ReadNode : public ASTNode {
private:
    shared_ptr<VariableNode> variableNode;

public:
    ReadNode(shared_ptr<VariableNode> variableNode);
    VariableNode getVariableNode();

};

#endif //SPA_READNODE_H
