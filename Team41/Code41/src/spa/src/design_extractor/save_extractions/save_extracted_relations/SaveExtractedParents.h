//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDPARENTS_H
#define SPA_SAVEEXTRACTEDPARENTS_H

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include "design_extractor/save_extractions/save_to_pkb/SaveToPKBInterface.h"

class SaveExtractedParents : public SaveToPKBInterface{
public:
    SaveExtractedParents(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveToPKBInterface(std::move(programNode),
                         std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDPARENTS_H
