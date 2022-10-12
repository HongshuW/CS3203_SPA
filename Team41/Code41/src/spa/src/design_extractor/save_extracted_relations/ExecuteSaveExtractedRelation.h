//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_EXECUTESAVEEXTRACTEDRELATION_H
#define SPA_EXECUTESAVEEXTRACTEDRELATION_H

#include "SaveExtractedRelation.h"
#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include <vector>

class ExecuteSaveExtractedRelation {
public:
    ExecuteSaveExtractedRelation() = default;
    ExecuteSaveExtractedRelation(shared_ptr<ProgramNode> programNode,
                                 shared_ptr<DataModifier> dataModifier);
    void executeSave();
private:
    vector<SaveExtractedRelation*> init(SaveExtractedRelation relations);
    shared_ptr<ProgramNode> programNode;
    shared_ptr<DataModifier> dataModifier;
};


#endif //SPA_EXECUTESAVEEXTRACTEDRELATION_H
