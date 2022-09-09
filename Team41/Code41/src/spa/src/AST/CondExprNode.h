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
        //TODO: to be implemented later using enum
        string left;
        string op;
        string right;

        CondExprNode(string left, string op, string right);
    };

} // AST

#endif //SPA_CONDEXPRNODE_H