//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDCALLS_H
#define SPA_SAVEEXTRACTEDCALLS_H

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include "design_extractor/save_extractions/save_to_pkb/SaveToPKBInterface.h"

class SaveExtractedCalls : public SaveToPKBInterface { ;
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

public:
    SaveExtractedCalls(shared_ptr<ProgramNode> programNode,
                       shared_ptr<DataModifier> dataModifier)
                       : SaveToPKBInterface(std::move(programNode),
                                            std::move(dataModifier)) {}
};


#endif //SPA_SAVEEXTRACTEDCALLS_H
