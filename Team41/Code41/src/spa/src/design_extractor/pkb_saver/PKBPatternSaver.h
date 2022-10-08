//
// Created by Aaron on 8/10/22.
//

#ifndef SPA_PKBPATTERNSAVER_H
#define SPA_PKBPATTERNSAVER_H

#include "../../AST/ProgramNode.h"
#include "../../PKB/DataModifier.h"


class PKBPatternSaver {
public:
    static void saveIfPattern(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier);
    static void saveWhilePattern(shared_ptr<ProgramNode> programNode,  shared_ptr<DataModifier> dataModifier);
    static void saveConditionalPatterns(shared_ptr<ProgramNode> programNode,  shared_ptr<DataModifier> dataModifier);
};


#endif //SPA_PKBPATTERNSAVER_H
