//
// Created by Aaron on 11/10/22.
//

#include "PKBSaverInterface.h"

#include <utility>

PKBSaverInterface::PKBSaverInterface(shared_ptr<ProgramNode> programNode,
                                     shared_ptr<DataModifier> dataModifier) {
    this->programNode = std::move(programNode);
    this->dataModifier = std::move(dataModifier);
}
