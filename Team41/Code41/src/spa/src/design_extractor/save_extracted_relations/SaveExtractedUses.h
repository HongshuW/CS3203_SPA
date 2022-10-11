//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDUSES_H
#define SPA_SAVEEXTRACTEDUSES_H

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include "SaveExtractedRelation.h"

class SaveExtractedUses : SaveExtractedRelation {
public:
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

};


#endif //SPA_SAVEEXTRACTEDUSES_H
