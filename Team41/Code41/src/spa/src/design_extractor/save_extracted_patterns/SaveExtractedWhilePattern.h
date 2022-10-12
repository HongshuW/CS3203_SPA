//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_SAVEEXTRACTEDWHILEPATTERN_H
#define SPA_SAVEEXTRACTEDWHILEPATTERN_H

#include "../save_extracted_design/SaveExtractedDesign.h"

class SaveExtractedWhilePattern : public SaveExtractedDesign {
public:
    SaveExtractedWhilePattern(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveExtractedDesign(std::move(programNode), std::move(dataModifier)) {}
private:
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};



#endif //SPA_SAVEEXTRACTEDWHILEPATTERN_H
