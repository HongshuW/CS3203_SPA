//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDIFPATTERN_H
#define SPA_SAVEEXTRACTEDIFPATTERN_H

#include "../save_extracted_design/SaveExtractedDesign.h"

class SaveExtractedIfPattern : public SaveExtractedDesign {
public:
    SaveExtractedIfPattern(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveExtractedDesign(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDIFPATTERN_H
