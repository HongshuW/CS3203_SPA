//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVETOPKBINTERFACE_H
#define SPA_SAVETOPKBINTERFACE_H

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"

class SaveToPKBInterface {
public:
    SaveToPKBInterface(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier);
    virtual void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) = 0;
private:
    shared_ptr<ProgramNode> programNode;
    shared_ptr<DataModifier> dataModifier;
};


#endif //SPA_SAVETOPKBINTERFACE_H
