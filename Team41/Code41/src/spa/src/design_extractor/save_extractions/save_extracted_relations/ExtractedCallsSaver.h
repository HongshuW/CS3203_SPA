//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_EXTRACTEDCALLSSAVER_H
#define SPA_EXTRACTEDCALLSSAVER_H

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include "design_extractor/save_extractions/save_to_pkb/PKBSaverInterface.h"

class ExtractedCallsSaver : public PKBSaverInterface { ;
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

public:
    ExtractedCallsSaver(shared_ptr<ProgramNode> programNode,
                        shared_ptr<DataModifier> dataModifier)
                       : PKBSaverInterface(std::move(programNode),
                                           std::move(dataModifier)) {}
};


#endif //SPA_EXTRACTEDCALLSSAVER_H
