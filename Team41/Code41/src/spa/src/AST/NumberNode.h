//
// Created by Tan Wei En on 2/9/22.
//

#include "ASTNode.h"

#ifndef SPA_NUMBERNODE_H
#define SPA_NUMBERNODE_H

class NumberNode : public ASTNode {
private:
    std::string number;
public:
    NumberNode(std::string number);
    std::string getNumber();
};

#endif //SPA_NUMBERNODE_H
