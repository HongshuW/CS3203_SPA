//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVEEXTRACTEDMODIFIES_H
#define SPA_SAVEEXTRACTEDMODIFIES_H

#include <utility>

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include "design_extractor/save_extractions/save_to_pkb/SaveToPKBInterface.h"

class SaveExtractedModifies : public SaveToPKBInterface {
public:
    SaveExtractedModifies(shared_ptr<ProgramNode> programNode,
                          shared_ptr<DataModifier> dataModifier)
                          : SaveToPKBInterface(std::move(programNode),
                                               std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDMODIFIES_H
