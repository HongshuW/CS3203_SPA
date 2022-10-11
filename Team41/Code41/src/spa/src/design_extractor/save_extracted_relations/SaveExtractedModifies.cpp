//
// Created by Aaron on 11/10/22.
//
#include "SaveExtractedModifies.h"

#include <utility>
#include "../ModifiesExtractor.h"

void SaveExtractedModifies::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    list<vector<string>> modifiesSList = *ModifiesExtractor::extractModifiesS(std::move(programNode));
    list<vector<string>> modifiesPList = *ModifiesExtractor::extractModifiesP(std::move(programNode));

    for (auto entry: modifiesSList) {
        dataModifier->saveModifiesS(entry);
    }

    for (auto entry: modifiesPList) {
        dataModifier->saveModifiesP(entry);
    }
}
