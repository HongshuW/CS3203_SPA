//
// Created by Aaron on 11/10/22.
//

#include "SaveExtractedRelationVisitor.h"

#include <utility>

SaveExtractedRelationVisitor::SaveExtractedRelationVisitor(shared_ptr<ProgramNode> programNode,
                                                           shared_ptr<DataModifier> dataModifier) {
    this->programNode = std::move(programNode);
    this->dataModifier = std::move(dataModifier);
}

void SaveExtractedRelationVisitor::visit(SaveExtractedModifies saveExtractedModifies) {
    SaveExtractedModifies::save(this->programNode, this->dataModifier);
}
