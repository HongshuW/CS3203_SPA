//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_EXECUTESAVEEXTRACTEDDESIGN_H
#define SPA_EXECUTESAVEEXTRACTEDDESIGN_H

#include "SaveExtractedDesign.h"
#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include <vector>

class ExecuteSaveExtractedDesign {
public:
    ExecuteSaveExtractedDesign(shared_ptr<ProgramNode> programNode,
                               shared_ptr<DataModifier> dataModifier);
    void executeSave();
private:
    vector<SaveExtractedDesign*> init();
    vector<shared_ptr<SaveExtractedDesign>> initRelations(vector<shared_ptr<SaveExtractedDesign>>& output);
    shared_ptr<ProgramNode> programNode;
    shared_ptr<DataModifier> dataModifier;
};


#endif //SPA_EXECUTESAVEEXTRACTEDDESIGN_H
