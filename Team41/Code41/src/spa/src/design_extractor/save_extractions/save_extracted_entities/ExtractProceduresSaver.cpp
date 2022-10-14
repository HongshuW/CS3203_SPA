//
// Created by Aaron on 14/10/22.
//

#include "ExtractProceduresSaver.h"
#include "design_extractor/EntityExtractor.h"

using namespace DE;

void ExtractProceduresSaver::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    shared_ptr<unordered_set<string>> set;
    auto procedureList = programNode->procedureList;
    for (const auto& procedureNode: procedureList) {
        set->insert(procedureNode->procedureName);
    }
    list<string> entityResultList;
    auto it = entityResultList.begin();
    for (const auto& elem: *set) {
        it = entityResultList.insert(it, elem);
    }

    dataModifier->saveProcedures(entityResultList);
}