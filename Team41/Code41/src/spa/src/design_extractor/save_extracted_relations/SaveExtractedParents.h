//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDPARENTS_H
#define SPA_SAVEEXTRACTEDPARENTS_H

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include "design_extractor/save_extracted_design/SaveExtractedDesign.h"

class SaveExtractedParents : public SaveExtractedDesign{
public:
    SaveExtractedParents(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveExtractedDesign(std::move(programNode),
            std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDPARENTS_H
