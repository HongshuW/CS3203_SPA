//
// Created by Tan Wei En on 3/9/22.
//

#include <vector>
#include "ASTNode.h"
#include "ProcedureNode.h"

#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

using namespace std;

class ProgramNode : public ASTNode {
private:
    vector<shared_ptr<ProcedureNode>> procedureList;
public:
    ProgramNode(vector<shared_ptr<ProcedureNode>> procedureList);
    vector<shared_ptr<ProcedureNode>> getProcedureList();
};

#endif //SPA_PROGRAMNODE_H
