//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_STMTNODE_H
#define SPA_STMTNODE_H

namespace AST {
    class StmtNode : public ASTNode {
    public:
        ~StmtNode() override = default;
        bool operator==(const ASTNode& node) const override = 0;
    };
}

#endif //SPA_STMTNODE_H
