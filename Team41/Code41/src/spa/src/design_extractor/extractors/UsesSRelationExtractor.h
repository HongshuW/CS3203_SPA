//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_USESSRELATIONEXTRACTOR_H
#define SPA_USESSRELATIONEXTRACTOR_H
#include "AST/CondExprNode.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/UsesRelationExtractor.h"

namespace DE {

class UsesSRelationExtractor : public UsesRelationExtractor {
  unordered_set<string> extractUsesSHelper(
      const shared_ptr<ASTNode>& node,
      const shared_ptr<vector<vector<string>>>& result,
      const shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>& stmtNumbers);
  unordered_map<string, unordered_set<string>> mapProceduresToUsedVariables(
      const shared_ptr<ProgramNode>& rootPtr);

  unordered_map<string, unordered_set<string>>
  mapIfAndWhileStmtNoToUsedVariables(const shared_ptr<ProgramNode>& rootPtr);

  void insertCallsInIfAndWhileForUseS(
      const shared_ptr<ProgramNode>& rootPtr,
      const shared_ptr<list<vector<string>>>& ans);

  void insertCallsForUseS(const shared_ptr<ProgramNode>& rootPtr,
                          shared_ptr<list<vector<string>>> ans);

  void extractCallStmtRelationshipsWithIfAndWhileToOutput(
      const shared_ptr<ProgramNode>& rootPtr,
      unordered_map<string, unordered_set<string>> mappedProceduresToVars,
      unordered_map<string, unordered_set<string>> mappedIfAndWhileToVars,
      const shared_ptr<list<vector<string>>>& output);

  void extractCallStmtRelationshipsToOutput(
      int stmtNo, const shared_ptr<CallNode>& callNode,
      unordered_map<string, unordered_set<string>> mappedProceduresToVar,
      unordered_map<string, vector<shared_ptr<CallNode>>>
          mappedCallNodesToProcedures,
      const shared_ptr<list<vector<string>>>& output);

 public:
  UsesSRelationExtractor(shared_ptr<DataModifier> dataModifier,
                         shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_USESSRELATIONEXTRACTOR_H
