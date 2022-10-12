//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVEEXTRACTEDMODIFIES_H
#define SPA_SAVEEXTRACTEDMODIFIES_H

#include <utility>

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include "SaveExtractedRelation.h"

class SaveExtractedModifies : public SaveExtractedRelation {
public:
    SaveExtractedModifies(shared_ptr<ProgramNode> programNode,
                          shared_ptr<DataModifier> dataModifier)
                          : SaveExtractedRelation(std::move(programNode),
                                                  std::move(dataModifier)) {};
    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_SAVEEXTRACTEDMODIFIES_H
