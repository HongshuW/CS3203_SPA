//
// Created by Aaron on 14/10/22.
//

#include "ExtractConstantSaver.h"
#include "design_extractor/EntityExtractor.h"

using namespace DE;

void ExtractConstantSaver::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    vector<shared_ptr<StmtNode>> stmtNodeList = EntityExtractor::extractStmtNodes(programNode);
    unordered_set<string> output = unordered_set<string>();
    for (const auto& stmtNode: stmtNodeList) {
        auto variableSet = EntityExtractor::extractConstants(stmtNode);
        output.insert(variableSet->begin(), variableSet->end());
    }

    list<string> entityResultList;
    auto it = entityResultList.begin();
    for (const auto& elem: output) {
        it = entityResultList.insert(it, elem);
    }

    dataModifier->saveConstants(entityResultList);
}