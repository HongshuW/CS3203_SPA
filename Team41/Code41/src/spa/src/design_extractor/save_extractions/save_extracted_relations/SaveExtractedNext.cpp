//
// Created by Aaron on 12/10/22.
//

#include "SaveExtractedNext.h"
#include "design_extractor/NextExtractor.h"

void SaveExtractedNext::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    list<vector<string>> nextList = *NextExtractor::extractNext(programNode);
    for (auto entry: nextList) {
        dataModifier->saveNext(entry);
    }
}
