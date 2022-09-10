//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_EXPRNODE_H
#define SPA_EXPRNODE_H

#include "ASTNode.h"
#include <string>


namespace AST {

    class ExprNode : public ASTNode {
        //TODO: implement a binary tree later
    public:
        string expr;

        ExprNode(string expr);

        bool operator==(const ASTNode& node) const {
            ExprNode castedNode = dynamic_cast<const ExprNode&>(node);
            return expr == castedNode.expr;
        }

    };

} // AST

#endif //SPA_EXPRNODE_H
