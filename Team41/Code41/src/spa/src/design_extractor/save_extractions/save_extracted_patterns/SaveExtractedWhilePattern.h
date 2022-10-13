//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDWHILEPATTERN_H
#define SPA_SAVEEXTRACTEDWHILEPATTERN_H

#include "design_extractor/save_extractions/save_to_pkb/SaveToPKBInterface.h"

class SaveExtractedWhilePattern : public SaveToPKBInterface {
public:
    SaveExtractedWhilePattern(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveToPKBInterface(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};



#endif //SPA_SAVEEXTRACTEDWHILEPATTERN_H
