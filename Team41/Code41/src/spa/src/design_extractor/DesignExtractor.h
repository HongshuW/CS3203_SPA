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
        void extractVariablesFromProcedure(shared_ptr<ProcedureNode> procedureNode,
                                           shared_ptr<unordered_set<string>> set);
        unordered_set<string> getVariablesFromExprString(string expr);
        bool is_number(const std::string& s);


    public:
        explicit DesignExtractor(DataModifier, shared_ptr<ProgramNode> programNode);

        shared_ptr<unordered_set<string>> extractVariables();
        void saveVariableToPKB();
    };

}

#endif //SPA_DESIGNEXTRACTOR_H
