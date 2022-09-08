//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include <memory>
#include <vector>
#include "CondExprNode.h"
#include "ASTNode.h"
#include "StmtNode.h"

using namespace std;
using namespace AST;

namespace AST {
    class WhileNode : public StmtNode {
    public:
        shared_ptr<CondExprNode> condExpr;
        vector<shared_ptr<StmtNode>> stmtList;

        WhileNode(shared_ptr<CondExprNode> condExpr, vector<shared_ptr<StmtNode>> stmtList);
    };

}

#endif //SPA_WHILENODE_H
