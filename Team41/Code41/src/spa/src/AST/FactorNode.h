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
    shared_ptr<VariableNode> variableNode = nullptr;
    shared_ptr<NumberNode> numberNode = nullptr
    shared_ptr<ExpressionNode> expressionNode = nullptr;
public:
    FactorNode();
    void setVariable(shared_ptr<VariableNode> variableNode);
    void setNumber(shared_ptr<NumberNode> numberNode);
    void setExpression(shared_ptr<ExpressionNode> expressionNode);
};

#endif //SPA_FACTORNODE_H
