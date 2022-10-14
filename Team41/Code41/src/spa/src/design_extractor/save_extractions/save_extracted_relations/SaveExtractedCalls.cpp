//
// Created by Aaron on 12/10/22.
//

#include "SaveExtractedCalls.h"
#include "design_extractor/CallsExtractor.h"

void SaveExtractedCalls::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    list<vector<string>> callsList = *CallsExtractor::extractCalls(programNode);
    list<vector<string>> callsStarList = *CallsExtractor::extractCallsStar(programNode);

    for (auto entry: callsList) {
        dataModifier->saveCalls(entry);
    }

    for (auto entry: callsStarList) {
        dataModifier->saveCallsT(entry);
    }
}
