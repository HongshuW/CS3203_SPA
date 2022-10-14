//
// Created by Aaron on 14/10/22.
//

#ifndef SPA_EXTRACTPROCEDURESSAVER_H
#define SPA_EXTRACTPROCEDURESSAVER_H

#include "../save_to_pkb/SaveToPKBInterface.h"

class ExtractProceduresSaver : public SaveToPKBInterface {
public:
    ExtractProceduresSaver(shared_ptr<ProgramNode> programNode,
    shared_ptr<DataModifier> dataModifier)
    : SaveToPKBInterface(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<AST::ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_EXTRACTPROCEDURESSAVER_H
