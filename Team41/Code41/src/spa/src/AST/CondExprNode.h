//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_CONDEXPRNODE_H
#define SPA_CONDEXPRNODE_H

#include <string>
#include "ASTNode.h"
#include "RelExprNode.h"

using namespace std;

namespace AST {

    class CondExprNode : public ASTNode {
    public:
        string condExpr;
        shared_ptr<RelExprNode> relExprNode = nullptr;
        shared_ptr<CondExprNode> condExprLHS = nullptr;
        string op = "";
        shared_ptr<CondExprNode> condExprRHS = nullptr;

        // string representation
        CondExprNode(string condExpr);

        // x == y
        CondExprNode(shared_ptr<RelExprNode> relExprNode);

        // ! (x == y)
        CondExprNode(shared_ptr<CondExprNode> condExprLHS);

        // (x == y) && (z == 1)
        CondExprNode(shared_ptr<CondExprNode> condExprLHS, string op, shared_ptr<CondExprNode> condExprRHS);

        bool operator==(const ASTNode& node) const {
            CondExprNode castedNode = dynamic_cast<const CondExprNode&>(node);
            return condExpr == castedNode.condExpr &&
                    *relExprNode == *castedNode.relExprNode &&
                    *condExprLHS == *castedNode.condExprLHS &&
                    *condExprRHS == *castedNode.condExprRHS &&
                    op == castedNode.op;
        }

    };

} // AST

#endif //SPA_CONDEXPRNODE_H
