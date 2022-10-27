//
// Created by Xingchen Lin on 26/10/22.
//

#ifndef SPA_DESIGNEXTRACTORUTILS_H
#define SPA_DESIGNEXTRACTORUTILS_H

#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "AST/CallNode.h"
#include "AST/ProgramNode.h"
#include "AST/StmtNode.h"
#include "AST/utils/ASTUtils.h"
#include "design_extractor/extractors/ModifiesSRelationExtractor.h"

using namespace std;

namespace DE {

class DesignExtractorUtils {
 public:
  DesignExtractorUtils();
  //! For calls
  static unordered_map<string, vector<shared_ptr<CallNode>>>
  extractCallNodesFromProcedures(const shared_ptr<ProgramNode>& rootPtr);

  static void extractVariablesFromCallNodesInProceduresToList(
      const shared_ptr<CallNode>& callNode,
      unordered_map<string, unordered_set<string>> mappedProceduresToVar,
      unordered_map<string, vector<shared_ptr<CallNode>>>
          mappedCallNodesToProcedures,
      unordered_set<string>& uniqueVarList);

  static vector<shared_ptr<StmtNode>> extractIfAndWhileNodesFromProcedures(
      const shared_ptr<ProgramNode>& rootPtr);

  static void extractCallStmtRelationshipsToOutput(
      int stmtNo, const shared_ptr<CallNode>& callNode,
      StrToSetMap mappedProceduresToVars,
      unordered_map<string, vector<shared_ptr<CallNode>>>
          mappedCallNodesToProcedures,
      const shared_ptr<list<vector<string>>>& output);

  static void extractCallStmtRelationshipsWithIfAndWhileToOutput(
      const shared_ptr<ProgramNode>& rootPtr,
      StrToSetMap mappedProceduresToVars, StrToSetMap mappedIfAndWhileToVars,
      const shared_ptr<list<vector<string>>>& output);
};

}  // namespace DE

#endif  // SPA_DESIGNEXTRACTORUTILS_H
