//
// Created by Aaron on 12/10/22.
//

#include "ExecuteSaveExtractedDesign.h"
#include "design_extractor/save_extracted_relations/SaveExtractedCalls.h"
#include "design_extractor/save_extracted_relations/SaveExtractedFollows.h"
#include "design_extractor/save_extracted_relations/SaveExtractedModifies.h"
#include "design_extractor/save_extracted_relations/SaveExtractedNext.h"
#include "design_extractor/save_extracted_relations/SaveExtractedUses.h"
#include "design_extractor/save_extracted_relations/SaveExtractedParents.h"

#include "design_extractor/save_extracted_patterns/SaveExtractedIfPattern.h"
#include "design_extractor/save_extracted_patterns/SaveExtractedWhilePattern.h"
#include "design_extractor/save_extracted_patterns/SaveExtractedAssignPattern.h"

ExecuteSaveExtractedDesign::ExecuteSaveExtractedDesign(shared_ptr<ProgramNode> programNode,
                                                       shared_ptr<DataModifier> dataModifier) {
    this->programNode = programNode;
    this->dataModifier = dataModifier;
}

void ExecuteSaveExtractedDesign::executeSave() {
    vector<shared_ptr<SaveExtractedDesign>> extractedDesignList;
    this->initRelations(extractedDesignList);
    this->initPatterns(extractedDesignList);
    for (const auto& design: extractedDesignList) {
        design->save(this->programNode, this->dataModifier);
    }
}

void ExecuteSaveExtractedDesign::initRelations(vector<shared_ptr<SaveExtractedDesign>>& extractedDesignList) {
    auto calls = make_shared<SaveExtractedCalls>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(calls);

    auto follows = make_shared<SaveExtractedFollows>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(follows);

    auto modifies = make_shared<SaveExtractedModifies>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(modifies);

    auto next = make_shared<SaveExtractedNext>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(next);

    auto uses = make_shared<SaveExtractedUses>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(uses);

    auto parents = make_shared<SaveExtractedParents>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(parents);
}

void ExecuteSaveExtractedDesign::initPatterns(vector<shared_ptr<SaveExtractedDesign>>& extractedDesignList) {
    auto assignPattern = make_shared<SaveExtractedAssignPattern>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(assignPattern);

    auto ifPattern = make_shared<SaveExtractedIfPattern>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(ifPattern);

    auto whilePattern = make_shared<SaveExtractedWhilePattern>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(whilePattern);
}


