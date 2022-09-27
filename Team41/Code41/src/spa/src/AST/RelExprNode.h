//
// Created by Xingchen Lin on 10/9/22.
//

#ifndef SPA_RELEXPRNODE_H
#define SPA_RELEXPRNODE_H

#include "ExprNode.h"

using namespace std;
using namespace AST;

namespace AST {
    class RelExprNode : public ASTNode {
    public:
        shared_ptr<ExprNode> exprNodeLHS;
        string op;
        shared_ptr<ExprNode> exprNodeRHS;

        RelExprNode(shared_ptr<ExprNode> exprNodeLHS, string op, shared_ptr<ExprNode> exprNodeRHS);

        bool operator==(const ASTNode& node) const override;

    };

} // AST

#endif //SPA_RELEXPRNODE_H
