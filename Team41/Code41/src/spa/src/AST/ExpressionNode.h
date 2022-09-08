//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_EXPRESSIONNODE_H
#define SPA_EXPRESSIONNODE_H

#include "ASTNode.h"
#include "TermNode.h"

using namespace std;

class ExpressionNode : public ASTNode {
private:
    shared_ptr<ExpressionNode> expressionNode;
    string op;
//    shared_ptr<TermNode> termNode;
};

#endif //SPA_EXPRESSIONNODE_H
