//
// Created by Tan Wei En on 2/9/22.
//

#include "ASTNode.h"

#ifndef SPA_NUMBERNODE_H
#define SPA_NUMBERNODE_H

using namespace std;

class NumberNode : public ASTNode {
private:
    string number;
public:
    NumberNode(string number);
    string getNumber();
};

#endif //SPA_NUMBERNODE_H
