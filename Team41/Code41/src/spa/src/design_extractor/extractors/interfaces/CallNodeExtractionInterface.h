//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_CALLNODEEXTRACTIONINTERFACE_H
#define SPA_CALLNODEEXTRACTIONINTERFACE_H
#include <string>
#include <unordered_map>

#include "AST/CallNode.h"
#include "AST/ProgramNode.h"
#include "design_extractor/utils/DesignExtractorUtils.h"

namespace DE {

class CallNodeExtractionInterface {
 public:
  unordered_map<string, vector<shared_ptr<CallNode>>>
  extractCallNodesFromProcedures(const shared_ptr<ProgramNode>& rootPtr);

  void extractCallStmtRelationshipsToOutput(
      int stmtNo, const shared_ptr<CallNode>& callNode,
      StrToSetMap mappedProceduresToVars,
      unordered_map<string, vector<shared_ptr<CallNode>>>
          mappedCallNodesToProcedures,
      const shared_ptr<list<vector<string>>>& output);

  vector<shared_ptr<StmtNode>> extractIfAndWhileNodesFromProcedures(
      const shared_ptr<ProgramNode>& rootPtr);

  void extractCallStmtRelationshipsWithIfAndWhileToOutput(
      const shared_ptr<ProgramNode>& rootPtr,
      StrToSetMap mappedProceduresToVars, StrToSetMap mappedIfAndWhileToVars,
      const shared_ptr<list<vector<string>>>& output);
};

}  // namespace DE

#endif  // SPA_CALLNODEEXTRACTIONINTERFACE_H
