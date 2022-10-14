//
// Created by Aaron on 14/10/22.
//

#ifndef SPA_EXTRACTCONSTANTSAVER_H
#define SPA_EXTRACTCONSTANTSAVER_H

#include "../save_to_pkb/SaveToPKBInterface.h"

class ExtractConstantSaver : public SaveToPKBInterface {
public:
    ExtractConstantSaver(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveToPKBInterface(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<AST::ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

};


#endif //SPA_EXTRACTCONSTANTSAVER_H
