//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_EXPRNODE_H
#define SPA_EXPRNODE_H

#include "ASTNode.h"
#include <string>
namespace AST {

    class ExprNode : public ASTNode {
        //TODO: to be implemented later
    public:
        string expression;
        explicit ExprNode(string expression);
    };

} // AST

#endif //SPA_EXPRNODE_H
