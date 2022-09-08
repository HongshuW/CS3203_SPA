//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_STMTNODE_H
#define SPA_STMTNODE_H

namespace AST {
    class StmtNode : public ASTNode {
    public:
        virtual ~StmtNode() = default;
    };
}

#endif //SPA_STMTNODE_H
