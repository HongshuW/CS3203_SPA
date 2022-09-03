//
// Created by Tan Wei En on 3/9/22.
//

#include <vector>
#include "ASTNode.h"
#include "ProcedureNode.h"

#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

class ProgramNode : public ASTNode {
private:
    std::vector<std::shared_ptr<ProcedureNode>> procedureList;
public:
    ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedureList);
};

#endif //SPA_PROGRAMNODE_H

