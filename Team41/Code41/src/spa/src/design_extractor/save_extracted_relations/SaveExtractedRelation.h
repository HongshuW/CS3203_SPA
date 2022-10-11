//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVEEXTRACTEDRELATION_H
#define SPA_SAVEEXTRACTEDRELATION_H

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"

class SaveExtractedRelation {
public:
    virtual ~SaveExtractedRelation() = default;
    virtual void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) = 0;
};


#endif //SPA_SAVEEXTRACTEDRELATION_H
