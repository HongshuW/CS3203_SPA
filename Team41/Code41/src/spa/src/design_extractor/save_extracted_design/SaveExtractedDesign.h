//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVEEXTRACTEDDESIGN_H
#define SPA_SAVEEXTRACTEDDESIGN_H

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"

class SaveExtractedDesign {
public:
    SaveExtractedDesign(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier);
    virtual void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) = 0;
private:
    shared_ptr<ProgramNode> programNode;
    shared_ptr<DataModifier> dataModifier;
};


#endif //SPA_SAVEEXTRACTEDDESIGN_H
