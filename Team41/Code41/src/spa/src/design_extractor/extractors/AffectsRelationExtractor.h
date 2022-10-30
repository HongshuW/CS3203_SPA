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
  bool checkNoWildcardDFS(int start, int end, CFG cfg);
  shared_ptr<vector<string>> extractNoWildcard(StmtNoArgs args);
  shared_ptr<vector<string>> extractWithStartGivenDFS(CFG cfg, int start);
  void extractWithEndBTHelper(CFG cfg, int stmt,
                              shared_ptr<unordered_set<string>> output,
                              shared_ptr<unordered_set<string>> usedVariables,
                              shared_ptr<unordered_set<int>> visited);
  shared_ptr<vector<string>> extractWithEndGivenBackTracking(CFG cfg, int end);
  shared_ptr<vector<string>> extractOneWildcard(StmtNoArgs args,
                                                bool isStartGiven);
  bool isModified(string variable, int stmtNo);
  bool areArgsValid(int start, int end);

 public:
  AffectsRelationExtractor(shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract(StmtNoArgs args) override;
  shared_ptr<ExtractorResult> extractAllRelations() override;
};

}  // namespace DE

#endif  // SPA_AFFECTSRELATIONEXTRACTOR_H
