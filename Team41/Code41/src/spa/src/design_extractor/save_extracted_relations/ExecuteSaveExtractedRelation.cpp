//
// Created by Aaron on 12/10/22.
//

#include "ExecuteSaveExtractedRelation.h"
#include "SaveExtractedCalls.h"
#include "SaveExtractedFollows.h"
#include "SaveExtractedModifies.h"
#include "SaveExtractedNext.h"
#include "SaveExtractedUses.h"

ExecuteSaveExtractedRelation::ExecuteSaveExtractedRelation(shared_ptr<ProgramNode> programNode,
                                                           shared_ptr<DataModifier> dataModifier) {
    this->programNode = programNode;
    this->dataModifier = dataModifier;
}

vector<SaveExtractedRelation*> ExecuteSaveExtractedRelation::init() {
    vector<SaveExtractedRelation*> extractedRelationList;

    auto* calls = new SaveExtractedCalls(programNode, dataModifier);
    extractedRelationList.push_back(calls);

    auto* follows = new SaveExtractedFollows(programNode, dataModifier);
    extractedRelationList.push_back(follows);

    auto* modifies = new SaveExtractedModifies(programNode, dataModifier);
    extractedRelationList.push_back(modifies);

    auto* next = new SaveExtractedNext(programNode, dataModifier);
    extractedRelationList.push_back(next);

    auto* uses = new SaveExtractedUses(programNode, dataModifier);
    extractedRelationList.push_back(uses);

    return extractedRelationList;
}

void ExecuteSaveExtractedRelation::executeSave() {
    vector<SaveExtractedRelation*> extractedRelationList = this->init();
    for (auto relation: extractedRelationList) {
        relation->save(this->programNode, this->dataModifier);
    }
}


