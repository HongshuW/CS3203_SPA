//
// Created by Tan Wei En on 3/9/22.
//

#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H

#include <list>
#include "AST/ProgramNode.h"
#include <unordered_map>
#include <unordered_set>
#include "pkb/DataModifier.h"

namespace DE {
    class DesignExtractor {
    private:
        shared_ptr<ProgramNode> programNode;
        DataModifier dataModifier;
        unordered_set<string> extractVariablesFromProcedure(shared_ptr<ProcedureNode> procedureNode);


    public:
        explicit DesignExtractor(DataModifier, shared_ptr<ProgramNode> programNode);

        std::list<string> extractVariables();
        void saveVariableToPKB();
    };

}

#endif //SPA_DESIGNEXTRACTOR_H
