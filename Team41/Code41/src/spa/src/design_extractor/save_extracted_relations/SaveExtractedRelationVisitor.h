//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVEEXTRACTEDRELATIONVISITOR_H
#define SPA_SAVEEXTRACTEDRELATIONVISITOR_H

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"
#include "SaveExtractedModifies.h"

class SaveExtractedRelationVisitor {
    shared_ptr<ProgramNode> programNode;
    shared_ptr<DataModifier> dataModifier;
    SaveExtractedRelationVisitor(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier);

    void visit(SaveExtractedModifies saveExtractedModifies);

};


#endif //SPA_SAVEEXTRACTEDRELATIONVISITOR_H
