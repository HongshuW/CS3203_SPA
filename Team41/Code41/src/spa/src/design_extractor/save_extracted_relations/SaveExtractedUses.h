//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDUSES_H
#define SPA_SAVEEXTRACTEDUSES_H

#include <utility>

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include "design_extractor/save_extracted_design/SaveExtractedDesign.h"

class SaveExtractedUses : public SaveExtractedDesign {
public:
    SaveExtractedUses(shared_ptr<ProgramNode> programNode,
                      shared_ptr<DataModifier> dataModifier)
                      : SaveExtractedDesign(std::move(programNode),
                                            std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

};


#endif //SPA_SAVEEXTRACTEDUSES_H
