//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDNEXT_H
#define SPA_SAVEEXTRACTEDNEXT_H

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include "design_extractor/save_extracted_design/SaveExtractedDesign.h"

class SaveExtractedNext : public SaveExtractedDesign {
public:
    SaveExtractedNext(shared_ptr<ProgramNode> programNode,
                      shared_ptr<DataModifier> dataModifier)
                      : SaveExtractedDesign(std::move(programNode),
                                            std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDNEXT_H
