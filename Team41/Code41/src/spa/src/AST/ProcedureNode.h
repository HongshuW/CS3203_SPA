//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

#include "ASTNode.h"
#include "AssignNode.h"
#include <vector>

using namespace std;
using namespace AST;

namespace AST {
    class ProcedureNode : public ASTNode {
    private:
        string procedureName;
        vector<shared_ptr<StmtNode>> stmtList;
    public:
        ProcedureNode(string procedureName, vector<shared_ptr<StmtNode>> stmtList);
    };
}

#endif //SPA_PROCEDURENODE_H
