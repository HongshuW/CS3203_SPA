//
// Created by Tan Wei En on 2/9/22.
//

#include "ASTNode.h"

#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

using namespace std;

class VariableNode : public ASTNode {
private:
    string variable;
public:
    VariableNode(string variable);
    string getVariable();
};

#endif //SPA_VARIABLENODE_H
