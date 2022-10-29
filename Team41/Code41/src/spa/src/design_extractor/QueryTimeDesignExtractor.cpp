//
// Created by Xingchen Lin on 27/10/22.
//

#include "QueryTimeDesignExtractor.h"

namespace DE {
QueryTimeDesignExtractor::QueryTimeDesignExtractor(
    shared_ptr<ProgramNode> programNode)
    : programNode(programNode) {
  stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
}

void QueryTimeDesignExtractor::clearCache() {}
}  // namespace DE