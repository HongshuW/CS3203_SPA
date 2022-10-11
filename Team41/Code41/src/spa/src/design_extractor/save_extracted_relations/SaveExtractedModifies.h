//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVEEXTRACTEDMODIFIES_H
#define SPA_SAVEEXTRACTEDMODIFIES_H

#include "../../AST/ProgramNode.h"
#include "../../pkb/DataModifier.h"

class SaveExtractedModifies {
public:
    static void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier);
};


#endif //SPA_SAVEEXTRACTEDMODIFIES_H
