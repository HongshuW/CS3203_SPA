//
// Created by Aaron on 14/10/22.
//

#include "ExtractVariableSaver.h"
#include "design_extractor/EntityExtractor.h"

using namespace DE;

void ExtractVariableSaver::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    vector<shared_ptr<StmtNode>> stmtNodeList = EntityExtractor::extractStmtNodes(programNode);
    unordered_set<string> output = *EntityExtractor::extractAllVariables(programNode);

    list<string> entityResultList;
    auto it = entityResultList.begin();
    for (const auto& elem: output) {
        it = entityResultList.insert(it, elem);
    }

    dataModifier->saveVariables(entityResultList);
}
