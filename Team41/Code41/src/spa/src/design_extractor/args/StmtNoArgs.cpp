//
// Created by Aaron on 5/10/22.
//

#include "StmtNoArgs.h"

StmtNoArgs::StmtNoArgs() = default;

int StmtNoArgs::getStartStmtNo() const { return startStmtNo; }

int StmtNoArgs::getEndStmtNo() const { return endStmtNo; }

void StmtNoArgs::setStartStmtNo(int start) { startStmtNo = start; }

void StmtNoArgs::setEndStmtNo(int end) { endStmtNo = end; }

void StmtNoArgs::setStartAndEndStmtNo(int start, int end) {
	startStmtNo = start;
	endStmtNo = end;
}

bool StmtNoArgs::startAndEndExists() const {
  return startStmtNo > 0 && endStmtNo > 0;
}

bool StmtNoArgs::startExistsOnly() const { return startStmtNo > 0 && endStmtNo == 0; }

bool StmtNoArgs::endExistsOnly() const { return startStmtNo == 0 && endStmtNo > 0; }
