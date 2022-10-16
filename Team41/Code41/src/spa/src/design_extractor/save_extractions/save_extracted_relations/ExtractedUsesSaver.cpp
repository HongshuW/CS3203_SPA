//
// Created by Aaron on 12/10/22.
//

#include "ExtractedUsesSaver.h"
#include "design_extractor/UsesExtractor.h"

using namespace DE;

void ExtractedUsesSaver::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    list<vector<string>> useSList = *UsesExtractor::extractUsesS(programNode);
    list<vector<string>> usePList = *UsesExtractor::extractUsesP(programNode);

    for (auto entry: useSList) {
        dataModifier->saveUsesS(entry);
    }

    for (auto entry: usePList) {
        dataModifier->saveUsesP(entry);
    }
}


