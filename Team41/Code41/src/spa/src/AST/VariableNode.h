//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

#include "ASTNode.h"
#include <string>

using namespace std;

namespace AST {
    class VariableNode : public ASTNode {
    public:
        string variable;

        VariableNode(string variable);

        bool operator==(const ASTNode& node) const {
            VariableNode castedNode = dynamic_cast<const VariableNode&>(node);
            return variable == castedNode.variable;
        }
    };
}

#endif //SPA_VARIABLENODE_H
