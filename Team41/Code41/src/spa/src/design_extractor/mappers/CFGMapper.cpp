//
// Created by Aaron on 22/10/22.
//

#include "CFGMapper.h"

#include <AST/utils/ASTUtils.h>

CFGMapper::CFGMapper(shared_ptr<ProgramNode> programNode) {
  this->programNode = programNode;
  initProcNodeToCFGMap();
}

void CFGMapper::initProcNodeToCFGMap() {
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(programNode);
  for (auto procNode : programNode->procedureList) {
    CFG cfg = CFG(*procNode, stmtNumbers);
    procNodeToCFGMap.insert(make_pair(procNode, cfg));
  }
}

unordered_map<shared_ptr<ProcedureNode>, CFG> CFGMapper::getProcNodeToCFGMap() {
  return procNodeToCFGMap;
}
