//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVEEXTRACTEDMODIFIES_H
#define SPA_SAVEEXTRACTEDMODIFIES_H

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include "SaveExtractedRelation.h"

class SaveExtractedModifies : SaveExtractedRelation {
public:
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDMODIFIES_H
