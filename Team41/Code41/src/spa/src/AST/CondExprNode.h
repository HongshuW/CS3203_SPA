//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_CONDEXPRNODE_H
#define SPA_CONDEXPRNODE_H

#include <string>
#include "ASTNode.h"

using namespace std;

namespace AST {

    class CondExprNode : public ASTNode {
    public:
        string condExpr;

        CondExprNode(string condExpr);

        bool operator==(const ASTNode& node) const {
            CondExprNode castedNode = dynamic_cast<const CondExprNode&>(node);
            return condExpr == castedNode.condExpr;
        }
    };

} // AST

#endif //SPA_CONDEXPRNODE_H
