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
#include "query_builder/commons/DesignEntity.h"

using namespace QB;
namespace DE {
    class DesignExtractor {
    private:
        shared_ptr<ProgramNode> programNode;
        unordered_map<int, vector<int>> parentRelations;
        DataModifier dataModifier;
        void
        extractEntitiesFromProcedure(shared_ptr<ProcedureNode> procedureNode, shared_ptr<unordered_set<string>> set,
                                     DesignEntity designEntityType);
        unordered_set<string> getVariablesFromExprString(string expr);
        bool is_number(const std::string& s);

    public:
        explicit DesignExtractor(DataModifier, shared_ptr<ProgramNode> programNode);

        unordered_map<int, vector<int>> extractParentRelations();
        shared_ptr<unordered_set<string>> extractEntities(DesignEntity designEntityType);
        void saveVariableToPKB();
        void saveParentToPKB();
    };

}

#endif //SPA_DESIGNEXTRACTOR_H
