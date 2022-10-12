//
// Created by Aaron on 11/10/22.
//
#include "SaveExtractedModifies.h"

#include <utility>
#include "design_extractor/ModifiesExtractor.h"

using namespace DE;

void SaveExtractedModifies::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    list<vector<string>> modifiesSList = *ModifiesExtractor::extractModifiesS(programNode);
    list<vector<string>> modifiesPList = *ModifiesExtractor::extractModifiesP(programNode);

    for (auto entry: modifiesSList) {
        dataModifier->saveModifiesS(entry);
    }

    for (auto entry: modifiesPList) {
        dataModifier->saveModifiesP(entry);
    }
}


