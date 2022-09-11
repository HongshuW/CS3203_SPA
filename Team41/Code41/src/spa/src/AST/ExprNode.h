//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_EXPRNODE_H
#define SPA_EXPRNODE_H

#include "ASTNode.h"
#include <string>

using namespace AST;
using namespace std;

namespace AST {

    class ExprNode : public ASTNode {
    public:
        string expr;
        shared_ptr<ExprNode> left;
        shared_ptr<ExprNode> right;

        ExprNode(string expr);

        bool operator==(const ASTNode& node) const {
            auto castedNode = dynamic_cast<const ExprNode*>(&node);

            return castedNode != 0
                && (left == castedNode->left || *left == *castedNode->left)
                && (right == castedNode->right || *right == *castedNode->right)
                && expr.compare(castedNode->expr) == 0;
        }
    };

} // AST

#endif //SPA_EXPRNODE_H
