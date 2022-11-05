//
// Created by Xingchen Lin on 28/10/22.
//

#ifndef SPA_AFFECTSRELATIONEXTRACTOR_H
#define SPA_AFFECTSRELATIONEXTRACTOR_H

#include <set>
#include <utility>

#include "../args/StmtNoArgs.h"
#include "AST/utils/ASTUtils.h"
#include "CFG/CFG.h"
#include "design_extractor/args/StmtNoArgs.h"
#include "design_extractor/extractors/AffectsCommonExtractor.h"
#include "design_extractor/results/RelationResult.h"

namespace DE {

class AffectsRelationExtractor : AffectsCommonExtractor {
 private:
  bool checkNoWildcardDFS(int start, int end, const CFG& cfg);
  shared_ptr<vector<string>> extractNoWildcard(StmtNoArgs args);
  shared_ptr<vector<string>> extractWithStartGivenDFS(const CFG& cfg,
                                                      int start);
  void extractWithEndBTHelper(
      const CFG& cfg, int stmt, const shared_ptr<unordered_set<string>>& output,
      const shared_ptr<unordered_set<string>>& usedVariables,
      const shared_ptr<unordered_set<int>>& visited);
  shared_ptr<vector<string>> extractWithEndGivenBackTracking(const CFG& cfg,
                                                             int end);
  shared_ptr<vector<string>> extractOneWildcard(StmtNoArgs args,
                                                bool isStartGiven);
  bool isModified(const string& variable, int stmtNo);
  bool areArgsValid(int start, int end);

 public:
  explicit AffectsRelationExtractor(shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract(StmtNoArgs args) override;
  shared_ptr<ExtractorResult> extractAllRelations() override;
};

}  // namespace DE

#endif  // SPA_AFFECTSRELATIONEXTRACTOR_H
