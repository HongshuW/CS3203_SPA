//
// Created by Nafour on 26/10/22.
//

#include "AbstractDesignExtractor.h"

#include <utility>

#include "AST/utils/ASTUtils.h"

namespace DE {
AbstractDesignExtractor::AbstractDesignExtractor(
    shared_ptr<DataModifier> dataModifier, const shared_ptr<ProgramNode>& programNode)
    : dataModifier(std::move(dataModifier)), programNode(programNode) {
  stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
}
}  // namespace DE
