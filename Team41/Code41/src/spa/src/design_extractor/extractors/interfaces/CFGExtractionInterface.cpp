//
// Created by Xingchen Lin on 28/10/22.
//

#include "CFGExtractionInterface.h"

namespace DE {
shared_ptr<ProcCFGMap> CFGExtractionInterface::generateProcCFGMap(
    shared_ptr<ProgramNode> programNode, shared_ptr<StmtNumbers> stmtNumbers) {
  shared_ptr<ProcCFGMap> procCFGMap = make_shared<ProcCFGMap>();
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  for (const shared_ptr<ProcedureNode>& procedure : procedureList) {
    CFG cfg = CFG(*procedure, stmtNumbers);
    procCFGMap->insert({procedure, cfg});
  }
  return procCFGMap;
}
}  // namespace DE