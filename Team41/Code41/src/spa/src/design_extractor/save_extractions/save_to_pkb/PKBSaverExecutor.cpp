//
// Created by Aaron on 12/10/22.
//

#include "PKBSaverExecutor.h"
#include "design_extractor/save_extractions/save_extracted_relations/ExtractedCallsSaver.h"
#include "design_extractor/save_extractions/save_extracted_relations/ExtractedFollowsSaver.h"
#include "design_extractor/save_extractions/save_extracted_relations/ExtractedModifiesSaver.h"
#include "design_extractor/save_extractions/save_extracted_relations/ExtractedNextSaver.h"
#include "design_extractor/save_extractions/save_extracted_relations/ExtractedUsesSaver.h"
#include "design_extractor/save_extractions/save_extracted_relations/ExtractedParentsSaver.h"

#include "design_extractor/save_extractions/save_extracted_patterns/ExtractedIfPatternSaver.h"
#include "design_extractor/save_extractions/save_extracted_patterns/ExtractedWhilePatternSaver.h"
#include "design_extractor/save_extractions/save_extracted_patterns/ExtractedAssignPatternSaver.h"

#include "design_extractor/save_extractions/save_extracted_entities/ExtractedVariableSaver.h"
#include "design_extractor/save_extractions/save_extracted_entities/ExtractedConstantSaver.h"
#include "design_extractor/save_extractions/save_extracted_entities/ExtractedProceduresSaver.h"

PKBSaverExecutor::PKBSaverExecutor(shared_ptr<ProgramNode> programNode,
                                   shared_ptr<DataModifier> dataModifier) {
    this->programNode = programNode;
    this->dataModifier = dataModifier;
}

void PKBSaverExecutor::executeSave() {
    vector<shared_ptr<PKBSaverInterface>> extractedDesignList;
    this->initRelations(extractedDesignList);
    this->initPatterns(extractedDesignList);
    this->initEntities(extractedDesignList);
    for (const auto& design: extractedDesignList) {
        design->save(this->programNode, this->dataModifier);
    }
}

void PKBSaverExecutor::initRelations(vector<shared_ptr<PKBSaverInterface>>& extractedDesignList) {
    auto calls = make_shared<ExtractedCallsSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(calls);

    auto follows = make_shared<ExtractedFollowsSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(follows);

    auto modifies = make_shared<ExtractedModifiesSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(modifies);

    auto next = make_shared<ExtractedNextSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(next);

    auto uses = make_shared<ExtractedUsesSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(uses);

    auto parents = make_shared<ExtractedParentsSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(parents);
}

void PKBSaverExecutor::initPatterns(vector<shared_ptr<PKBSaverInterface>>& extractedDesignList) {
    auto assignPattern = make_shared<ExtractedAssignPatternSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(assignPattern);

    auto ifPattern = make_shared<ExtractedIfPatternSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(ifPattern);

    auto whilePattern = make_shared<ExtractedWhilePatternSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(whilePattern);
}

void PKBSaverExecutor::initEntities(vector<shared_ptr<PKBSaverInterface>>& extractedDesignList) {
    auto variables = make_shared<ExtractedVariableSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(variables);

    auto constants = make_shared<ExtractedConstantSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(constants);

    auto procedures = make_shared<ExtractedProceduresSaver>(this->programNode, this->dataModifier);
    extractedDesignList.push_back(procedures);
}

