//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDFOLLOWS_H
#define SPA_SAVEEXTRACTEDFOLLOWS_H

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include "design_extractor/save_extractions/save_extracted_design/SaveExtractedDesign.h"

class SaveExtractedFollows : public SaveExtractedDesign { ;
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

public:
    SaveExtractedFollows(shared_ptr<ProgramNode> programNode,
                         shared_ptr<DataModifier> dataModifier)
                         : SaveExtractedDesign(std::move(programNode), std::move(dataModifier)) {}
};


#endif //SPA_SAVEEXTRACTEDFOLLOWS_H
