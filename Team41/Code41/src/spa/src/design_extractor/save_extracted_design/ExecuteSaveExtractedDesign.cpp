//
// Created by Aaron on 12/10/22.
//

#include "ExecuteSaveExtractedDesign.h"
#include "design_extractor/save_extracted_relations/SaveExtractedCalls.h"
#include "design_extractor/save_extracted_relations/SaveExtractedFollows.h"
#include "design_extractor/save_extracted_relations/SaveExtractedModifies.h"
#include "design_extractor/save_extracted_relations/SaveExtractedNext.h"
#include "design_extractor/save_extracted_relations/SaveExtractedUses.h"

ExecuteSaveExtractedDesign::ExecuteSaveExtractedDesign(shared_ptr<ProgramNode> programNode,
                                                       shared_ptr<DataModifier> dataModifier) {
    this->programNode = programNode;
    this->dataModifier = dataModifier;
}

vector<SaveExtractedDesign*> ExecuteSaveExtractedDesign::init() {
    vector<SaveExtractedDesign*> extractedRelationList;

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

void ExecuteSaveExtractedDesign::executeSave() {
    vector<shared_ptr<SaveExtractedDesign>> output;
    vector<shared_ptr<SaveExtractedDesign>> extractedDesignList = this->initRelations(output);
    for (const auto& design: extractedDesignList) {
        design->save(this->programNode, this->dataModifier);
    }
}

vector<shared_ptr<SaveExtractedDesign>> ExecuteSaveExtractedDesign::initRelations(vector<shared_ptr<SaveExtractedDesign>>& output) {
    auto calls = make_shared<SaveExtractedCalls>(this->programNode, this->dataModifier);
    output.push_back(calls);

    auto follows = make_shared<SaveExtractedFollows>(this->programNode, this->dataModifier);
    output.push_back(follows);

    auto modifies = make_shared<SaveExtractedModifies>(this->programNode, this->dataModifier);
    output.push_back(modifies);

    auto next = make_shared<SaveExtractedNext>(this->programNode, this->dataModifier);
    output.push_back(next);

    auto uses = make_shared<SaveExtractedUses>(this->programNode, this->dataModifier);
    output.push_back(uses);

    return output;
}


