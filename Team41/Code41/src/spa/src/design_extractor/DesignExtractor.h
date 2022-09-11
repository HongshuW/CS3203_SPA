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
#include "query_builder/commons/RelationType.h"

using namespace QB;
namespace DE {
    class DesignExtractor {
    private:
        shared_ptr<ProgramNode> programNode;
        DataModifier dataModifier;
        void
        extractEntitiesFromProcedure(shared_ptr<ProcedureNode> procedureNode, shared_ptr<unordered_set<string>> set,
                                     DesignEntity designEntityType);
        void saveEntityToPKB(DesignEntity designEntity);

    public:
        explicit DesignExtractor(DataModifier, shared_ptr<ProgramNode> programNode);
        shared_ptr<unordered_set<string>> extractEntities(DesignEntity designEntityType);
        void run();
        shared_ptr<list<vector<string>>> extractRelations(RelationType relationType);
        void saveRelationToPKB(RelationType relationType);
        vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>> extractPatterns();
    };
}

#endif //SPA_DESIGNEXTRACTOR_H
