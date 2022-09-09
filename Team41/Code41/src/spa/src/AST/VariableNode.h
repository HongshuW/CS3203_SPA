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
    private:
        string variable;
    public:
        VariableNode(string variable);
    };
}

#endif //SPA_VARIABLENODE_H