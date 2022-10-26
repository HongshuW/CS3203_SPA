//
// Created by Nafour on 26/10/22.
//

#include "AbstractDesignExtractor.h"
#include "AST/utils/ASTUtils.h"

namespace DE {
    AbstractDesignExtractor::AbstractDesignExtractor(shared_ptr<DataModifier> dataModifier,
                                                     shared_ptr<ProgramNode> programNode) :
                                                     dataModifier(dataModifier),
                                                     programNode(programNode) {
        stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
    }
} // DE