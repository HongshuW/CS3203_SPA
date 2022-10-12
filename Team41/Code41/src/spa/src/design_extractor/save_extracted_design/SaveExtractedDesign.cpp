//
// Created by Aaron on 11/10/22.
//

#include "SaveExtractedDesign.h"

#include <utility>

SaveExtractedDesign::SaveExtractedDesign(shared_ptr<ProgramNode> programNode,
                                         shared_ptr<DataModifier> dataModifier) {
    this->programNode = std::move(programNode);
    this->dataModifier = std::move(dataModifier);
}
