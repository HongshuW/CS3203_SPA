//
// Created by Xingchen Lin on 27/10/22.
//

#include "QueryTimeDesignExtractor.h"

namespace DE {
QueryTimeDesignExtractor::QueryTimeDesignExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : dataModifier(dataModifier), programNode(programNode) {
  stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
}
}  // namespace DE