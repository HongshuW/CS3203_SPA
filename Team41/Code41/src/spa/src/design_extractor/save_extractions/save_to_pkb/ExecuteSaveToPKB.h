//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_EXECUTESAVETOPKB_H
#define SPA_EXECUTESAVETOPKB_H

#include "SaveToPKBInterface.h"
#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include <vector>

class ExecuteSaveToPKB {
public:
    ExecuteSaveToPKB(shared_ptr<ProgramNode> programNode,
                     shared_ptr<DataModifier> dataModifier);
    void executeSave();
private:
    void initRelations(vector<shared_ptr<SaveToPKBInterface>>& extractedDesignList);
    void initPatterns(vector<shared_ptr<SaveToPKBInterface>>& extractedDesignList);

    shared_ptr<ProgramNode> programNode;
    shared_ptr<DataModifier> dataModifier;
};


#endif //SPA_EXECUTESAVETOPKB_H
