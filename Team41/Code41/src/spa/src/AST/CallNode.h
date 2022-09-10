//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include <memory>
#include "VariableNode.h"
#include "ASTNode.h"
#include "StmtNode.h"

using namespace std;

namespace AST {

    class CallNode : public StmtNode {
    public:
        string procedureName;

        explicit CallNode(string programName);

        bool operator==(const ASTNode& node) const {
            CallNode castedNode = dynamic_cast<const CallNode&>(node);
            return procedureName == castedNode.procedureName;
        }
    };

} // AST

#endif //SPA_CALLNODE_H
