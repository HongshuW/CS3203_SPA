//
// Created by Aaron on 1/10/22.
//

#include "NextStmtNoArgs.h"
#include "../../AST/utils/ASTUtils.h"

NextStmtNoArgs::NextStmtNoArgs()
{
}

bool NextStmtNoArgs::isBothArgsVaild(shared_ptr<ProgramNode> programNode, NextStmtNoArgs args)
{
    bool isValid = true;
    int start = args.getStartStmtNo();
    int end = args.getEndStmtNo();

    auto stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);
    bool startAndEndDoesNotExist = stmtNoToProcMap->count(start) == 0 && stmtNoToProcMap->count(end) == 0;
   

    if (startAndEndDoesNotExist) {
        isValid = false;
    }

    else {
        bool startAndEndSameProcedure = stmtNoToProcMap->at(start) == stmtNoToProcMap->at(end);
        if (!startAndEndSameProcedure) {
            isValid = false;
        }
    }

    return isValid;
}
