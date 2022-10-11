//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_EXPRNODE_H
#define SPA_EXPRNODE_H

#include "ASTNode.h"
#include "utils/Utils.h"
#include <string>

using namespace AST;
using namespace std;

namespace AST {

    class ExprNode : public ASTNode {
    public:
        string expr;
        shared_ptr<ExprNode> left;
        shared_ptr<ExprNode> right;

        explicit ExprNode(string expr);

        bool isVariableNode() const;
        bool isOperatorNode() const;

        bool operator==(const ASTNode& node) const override;
    };

} // AST

#endif //SPA_EXPRNODE_H
