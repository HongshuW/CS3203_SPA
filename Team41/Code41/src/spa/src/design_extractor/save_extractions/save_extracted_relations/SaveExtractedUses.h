//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDUSES_H
#define SPA_SAVEEXTRACTEDUSES_H

#include <utility>

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include "design_extractor/save_extractions/save_to_pkb/SaveToPKBInterface.h"

class SaveExtractedUses : public SaveToPKBInterface {
public:
    SaveExtractedUses(shared_ptr<ProgramNode> programNode,
                      shared_ptr<DataModifier> dataModifier)
                      : SaveToPKBInterface(std::move(programNode),
                                           std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

};


#endif //SPA_SAVEEXTRACTEDUSES_H
