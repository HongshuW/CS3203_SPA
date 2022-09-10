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
            ExprNode castedNode = dynamic_cast<const ExprNode&>(node);
            if (left == nullptr && castedNode.left == nullptr) {
                return true;
            }
            if (right == nullptr && castedNode.right == nullptr) {
                return true;
            }
            if (left == nullptr || castedNode.left == nullptr) {
                return false;
            }
            if (right == nullptr || castedNode.right == nullptr) {
                return false;
            }
            return expr == castedNode.expr && *left == *castedNode.left && *right == *castedNode.right;
        }
    };

} // AST

#endif //SPA_EXPRNODE_H
