//
// Created by Aaron on 12/10/22.
//

#include "ExtractedParentsSaver.h"
#include "design_extractor/ParentExtractor.h"

using namespace DE;

void ExtractedParentsSaver::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    list<vector<string>> parentList = *ParentExtractor::extractParent(programNode);
    list<vector<string>> parentStarList = *ParentExtractor::extractParentT(programNode);

    for (auto entry: parentList) {
        dataModifier->saveParent(entry);
    }

    for (auto entry: parentStarList) {
        dataModifier->saveParentT(entry);
    }
}
