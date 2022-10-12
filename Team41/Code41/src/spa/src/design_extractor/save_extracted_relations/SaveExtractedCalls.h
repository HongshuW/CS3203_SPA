//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDCALLS_H
#define SPA_SAVEEXTRACTEDCALLS_H

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include "SaveExtractedRelation.h"

class SaveExtractedCalls : public SaveExtractedRelation { ;
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

public:
    SaveExtractedCalls(shared_ptr<ProgramNode> programNode,
                       shared_ptr<DataModifier> dataModifier)
                       : SaveExtractedRelation(std::move(programNode),
                                               std::move(dataModifier)) {}
};


#endif //SPA_SAVEEXTRACTEDCALLS_H
