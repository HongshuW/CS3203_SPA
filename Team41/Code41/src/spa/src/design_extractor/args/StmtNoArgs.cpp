//
// Created by Aaron on 5/10/22.
//

#include "StmtNoArgs.h"
StmtNoArgs::StmtNoArgs(){};

bool StmtNoArgs::startAndEndExists() {
  return startStmtNo > 0 && endStmtNo > 0;
}

bool StmtNoArgs::startExistsOnly() { return startStmtNo > 0 && endStmtNo == 0; }

bool StmtNoArgs::endExistsOnly() { return startStmtNo == 0 && endStmtNo > 0; }

bool StmtNoArgs::noArgsProvided() { return startStmtNo == 0 && endStmtNo == 0; }
