//
// Created by Nafour on 26/10/22.
//

#include "EntitiesExtractor.h"

namespace DE {
EntitiesExtractor::EntitiesExtractor(shared_ptr<DataModifier> dataModifier,
                                     shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(dataModifier, programNode) {}

void EntitiesExtractor::extract() {}
}  // namespace DE