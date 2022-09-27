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
        explicit VariableNode(string variable);

        bool operator==(const ASTNode& node) const override;
    };
}

#endif //SPA_VARIABLENODE_H
