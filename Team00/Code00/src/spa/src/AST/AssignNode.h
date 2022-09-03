//
// Created by Tan Wei En on 3/9/22.
//

#include "ASTNode.h"
#include "VariableNode.h"
#include "NumberNode.h"

#ifndef SPA_ASSIGNNODE_H
#define SPA_ASSIGNNODE_H

using namespace std;

class AssignNode : public ASTNode {
private:
    shared_ptr<VariableNode> variableNode;
    // only allow for numbers now ie x = 1;, will change to expression in future
    shared_ptr<NumberNode> numberNode;

public:
    AssignNode(shared_ptr<VariableNode> variableNode, shared_ptr<NumberNode> numberNode);
};

#endif //SPA_ASSIGNNODE_H

