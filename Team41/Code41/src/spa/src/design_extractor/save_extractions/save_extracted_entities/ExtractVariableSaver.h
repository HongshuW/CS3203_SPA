//
// Created by Aaron on 14/10/22.
//

#ifndef SPA_EXTRACTVARIABLESAVER_H
#define SPA_EXTRACTVARIABLESAVER_H

#include "../save_to_pkb/SaveToPKBInterface.h"

class ExtractVariableSaver : public SaveToPKBInterface {
    ExtractVariableSaver(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveToPKBInterface(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_EXTRACTVARIABLESAVER_H
