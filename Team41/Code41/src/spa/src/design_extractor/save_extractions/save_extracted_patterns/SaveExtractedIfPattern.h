//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDIFPATTERN_H
#define SPA_SAVEEXTRACTEDIFPATTERN_H

#include "design_extractor/save_extractions/save_to_pkb/SaveToPKBInterface.h"

class SaveExtractedIfPattern : public SaveToPKBInterface {
public:
    SaveExtractedIfPattern(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveToPKBInterface(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDIFPATTERN_H
