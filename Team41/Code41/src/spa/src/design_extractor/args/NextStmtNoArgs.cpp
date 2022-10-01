//
// Created by Aaron on 1/10/22.
//

#include "NextStmtNoArgs.h"
#include "../../AST/utils/ASTUtils.h"

NextStmtNoArgs::NextStmtNoArgs()
{
}

bool NextStmtNoArgs::isArgVaild(shared_ptr<ProgramNode> programNode)
{
    bool isValid = true;
    int start = this->startStmtNo;
    int end = this->endStmtNo;

    auto stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);
    bool startAndEndDoesNotExist = stmtNoToProcMap->count(start) == 0 && stmtNoToProcMap->count(end) == 0;
    bool startOrEndDoesNotExist = stmtNoToProcMap->count(start) == 0 || stmtNoToProcMap->count(end) == 0;
   

    if (startAndEndDoesNotExist || startOrEndDoesNotExist) {
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
