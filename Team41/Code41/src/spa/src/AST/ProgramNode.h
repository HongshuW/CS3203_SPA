//
// Created by Tan Wei En on 3/9/22.
//

#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

#include <vector>
#include "ASTNode.h"
#include "ProcedureNode.h"

using namespace std;

namespace AST {
    class ProgramNode : public ASTNode {
    private:
    public:
    public:
        vector<shared_ptr<ProcedureNode>> procedureList;

        explicit ProgramNode(vector<shared_ptr<ProcedureNode>> procedureList);

        bool operator==(const ASTNode& node) const {
            ProgramNode castedNode = dynamic_cast<const ProgramNode&>(node);
            bool isEqual = equal(
                    begin(procedureList), end(procedureList),
                    begin(castedNode.procedureList), end(castedNode.procedureList),
                    [] (const shared_ptr<ProcedureNode> lhs, const shared_ptr<ProcedureNode> rhs)
                    {return *lhs == *rhs; });
            return isEqual;
        }
    };
}

#endif //SPA_PROGRAMNODE_H
