//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDNEXT_H
#define SPA_SAVEEXTRACTEDNEXT_H

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include "design_extractor/save_extractions/save_to_pkb/SaveToPKBInterface.h"

class SaveExtractedNext : public SaveToPKBInterface {
public:
    SaveExtractedNext(shared_ptr<ProgramNode> programNode,
                      shared_ptr<DataModifier> dataModifier)
                      : SaveToPKBInterface(std::move(programNode),
                                           std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDNEXT_H
