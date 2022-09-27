//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include <memory>
#include <vector>
#include "CondExprNode.h"
#include "ASTNode.h"
#include "StmtNode.h"

using namespace std;
using namespace AST;

namespace AST {

    class IfNode : public StmtNode {
    public:
        shared_ptr<CondExprNode> condExpr;
        vector<shared_ptr<StmtNode>> ifStmtList;
        vector<shared_ptr<StmtNode>> elseStmtList;

        /**
         *
         * @param condExpr
         * @param ifStmtList
         * @param elseStmtList
         */
        IfNode(shared_ptr<CondExprNode> condExpr,
               vector<shared_ptr<StmtNode>> ifStmtList,
               vector<shared_ptr<StmtNode>> elseStmtList);

        bool operator==(const ASTNode& node) const override;
    };

} // AST

#endif //SPA_IFNODE_H
