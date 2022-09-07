//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_FACTORNODE_H
#define SPA_FACTORNODE_H

#include "ASTNode.h"
#include "VariableNode.h"
#include "ExpressionNode.h"
#include "NumberNode.h"

class FactorNode : public ASTNode {
private:
    shared_ptr<VariableNode> variableNode;
    shared_ptr<NumberNode> numberNode;
    shared_ptr<ExpressionNode> expressionNode;
};

#endif //SPA_FACTORNODE_H
