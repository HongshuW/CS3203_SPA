//
// Created by Xingchen Lin on 28/10/22.
//

#ifndef SPA_CFGEXTRACTIONINTERFACE_H
#define SPA_CFGEXTRACTIONINTERFACE_H

#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "AST/CallNode.h"
#include "AST/ProgramNode.h"
#include "AST/StmtNode.h"
#include "AST/utils/ASTUtils.h"
#include "CFG/CFG.h"

namespace DE {

typedef unordered_map<shared_ptr<ProcedureNode>, CFG> ProcCFGMap;
typedef unordered_map<shared_ptr<StmtNode>, int> StmtNumbers;

class CFGExtractionInterface {
 protected:
  shared_ptr<ProcCFGMap> generateProcCFGMap(
      const shared_ptr<ProgramNode>& programNode,
      const shared_ptr<StmtNumbers>& stmtNumbers);
};

}  // namespace DE

#endif  // SPA_CFGEXTRACTIONINTERFACE_H
