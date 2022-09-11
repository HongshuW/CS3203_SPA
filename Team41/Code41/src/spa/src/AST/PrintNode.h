//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H

#include "ASTNode.h"
#include "VariableNode.h"
#include "StmtNode.h"

using namespace std;

namespace AST {
    class PrintNode : public StmtNode {
    public:
        shared_ptr<VariableNode> variableNode;

        PrintNode(shared_ptr<VariableNode> variableNode);

        bool operator==(const ASTNode& node) const {
            auto castedNode = dynamic_cast<const PrintNode*>(&node);
            return castedNode != 0 && *variableNode == *castedNode->variableNode;
        }

    };
}

#endif //SPA_PRINTNODE_H
