//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_EXTRACTEDMODIFIESSAVER_H
#define SPA_EXTRACTEDMODIFIESSAVER_H

#include <utility>

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"
#include "design_extractor/save_extractions/save_to_pkb/PKBSaverInterface.h"

class ExtractedModifiesSaver : public PKBSaverInterface {
public:
    ExtractedModifiesSaver(shared_ptr<ProgramNode> programNode,
                           shared_ptr<DataModifier> dataModifier)
                          : PKBSaverInterface(std::move(programNode),
                                              std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_EXTRACTEDMODIFIESSAVER_H
