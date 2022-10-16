//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_EXTRACTEDIFPATTERNSAVER_H
#define SPA_EXTRACTEDIFPATTERNSAVER_H

#include "design_extractor/save_extractions/save_to_pkb/PKBSaverInterface.h"

class ExtractedIfPatternSaver : public PKBSaverInterface {
public:
    ExtractedIfPatternSaver(shared_ptr<ProgramNode> programNode,
                            shared_ptr<DataModifier> dataModifier)
    : PKBSaverInterface(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_EXTRACTEDIFPATTERNSAVER_H
