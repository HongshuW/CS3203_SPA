//
// Created by Tan Wei En on 2/9/22.
//

#include "ASTNode.h"

#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

class VariableNode : public ASTNode {
private:
    std::string variable;
public:
    VariableNode(std::string variable);
};

#endif //SPA_VARIABLENODE_H

