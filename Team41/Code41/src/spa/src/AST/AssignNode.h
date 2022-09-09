//
// Created by Tan Wei En on 3/9/22.
//

#ifndef SPA_ASSIGNNODE_H
#define SPA_ASSIGNNODE_H

#include "ASTNode.h"
#include "VariableNode.h"
#include "ExprNode.h"
#include "StmtNode.h"

using namespace std;
using namespace AST;

namespace AST {
    class AssignNode : public StmtNode {
    private:
        shared_ptr<VariableNode> variableNode;
        // only allow for numbers now ie x = 1;, will change to expression in future
        shared_ptr<ExprNode> expressionNode;

    public:
        AssignNode(shared_ptr<VariableNode> variableNode, shared_ptr<ExprNode> expressionNode);
    };
}

#endif //SPA_ASSIGNNODE_H
