//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_READNODE_H
#define SPA_READNODE_H

#include "ASTNode.h"
#include "VariableNode.h"
#include "StmtNode.h"

using namespace std;
using namespace AST;

namespace AST {
    class ReadNode : public StmtNode {
    private:
        shared_ptr<VariableNode> variableNode;

    public:
        ReadNode(shared_ptr<VariableNode> variableNode);
    };
}

#endif //SPA_READNODE_H
