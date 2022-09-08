//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include <memory>
#include "VariableNode.h"
#include "ASTNode.h"
#include "StmtNode.h"

using namespace std;

namespace AST {

    class CallNode : public StmtNode {
    public:
        shared_ptr<VariableNode> variableNode;

        explicit CallNode(shared_ptr<VariableNode> variableNode);
    };

} // AST

#endif //SPA_CALLNODE_H
