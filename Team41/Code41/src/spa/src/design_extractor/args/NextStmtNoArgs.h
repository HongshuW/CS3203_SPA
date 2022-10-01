//
// Created by Aaron on 1/10/22.
//

#ifndef SPA_NEXTSTMTNOARGS_H
#define SPA_NEXTSTMTNOARGS_H

#include "../../AST/ProgramNode.h"


class NextStmtNoArgs {
public:
    NextStmtNoArgs();
    int getStartStmtNo() { return startStmtNo; };
    int getEndStmtNo() { return endStmtNo; }

    void setStartStmtNo(int start) {startStmtNo = start; };
    void setEndStmtNo(int end) {endStmtNo = end; };

    bool isBothArgsVaild(shared_ptr<ProgramNode> programNode, NextStmtNoArgs args);

private:
    int startStmtNo = 0;
    int endStmtNo = 0;
};


#endif //SPA_NEXTSTMTNOARGS_H
