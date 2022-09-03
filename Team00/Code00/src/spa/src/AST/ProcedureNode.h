//
// Created by Tan Wei En on 2/9/22.
//

#include "ASTNode.h"
#include "AssignNode.h"

#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

class ProcedureNode : public ASTNode {
private:
    std::string procedureName;
    // will change to statement list in the future
    std::shared_ptr<AssignNode> assignNode;
public:
    ProcedureNode(std::string procedureName, std::shared_ptr<AssignNode> assignNode);
};

#endif //SPA_PROCEDURENODE_H

