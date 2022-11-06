//
// Created by Xingchen Lin on 27/10/22.
//

#include "QueryTimeDesignExtractor.h"

namespace DE {
QueryTimeDesignExtractor::QueryTimeDesignExtractor(
    const shared_ptr<ProgramNode>& programNode)
    : programNode(programNode) {
  stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
}

bool QueryTimeDesignExtractor::isArgsWithinRange(StmtNoArgs args) {
  int lo = 1;
  auto hi = stmtNumbers->size();
  bool isStartInRange = true;
  bool isEndInRange = true;
  if (args.startAndEndExists()) {
    isStartInRange = args.getStartStmtNo() >= lo && args.getStartStmtNo() <= hi;
    isEndInRange = args.getEndStmtNo() >= lo && args.getEndStmtNo() <= hi;
  } else if (args.startExistsOnly()) {
    isStartInRange = args.getStartStmtNo() >= lo && args.getStartStmtNo() <= hi;
  } else if (args.endExistsOnly()) {
    isEndInRange = args.getEndStmtNo() >= lo && args.getEndStmtNo() <= hi;
  }
  return isStartInRange && isEndInRange;
}

}  // namespace DE