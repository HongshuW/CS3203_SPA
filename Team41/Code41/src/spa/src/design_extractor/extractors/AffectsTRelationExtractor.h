//
// Created by Nafour on 29/10/22.
//

#ifndef SPA_AFFECTSTRELATIONEXTRACTOR_H
#define SPA_AFFECTSTRELATIONEXTRACTOR_H
#include "design_extractor/extractors/AffectsCommonExtractor.h"

namespace DE {

class AffectsTRelationExtractor : public AffectsCommonExtractor {
  typedef unordered_map<int, unordered_set<int>> AffectsAdjList;

 private:
  vector<vector<bool>> dp;
  shared_ptr<AffectsAdjList> affectsAdjListPtr;
  shared_ptr<AffectsAdjList> affectsAdjListReversedPtr;

  shared_ptr<vector<string>> extractNoWildcard(int start, int end);
  bool extractNoWildcardDFS(int curr, int target,
                            shared_ptr<unordered_set<int>> visited);

  shared_ptr<vector<string>> extractWithStartGiven(int start);
  void extractWithStartGivenDFS(int curr, int start,
                                shared_ptr<vector<string>> result,
                                shared_ptr<unordered_set<int>> visited);

  shared_ptr<vector<string>> extractWithEndGiven(int end);
  void extractWithEndGivenDFS(int curr, int end,
                              shared_ptr<vector<string>> result,
                              shared_ptr<unordered_set<int>> visited);
  void initAffectsAdjList();
  void initAffectsAdjListReversed();

 public:
  AffectsTRelationExtractor(shared_ptr<DataModifier> dataModifier,
                            shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract(StmtNoArgs args) override;
  shared_ptr<ExtractorResult> extractAllRelations() override;
};

}  // namespace DE

#endif  // SPA_AFFECTSTRELATIONEXTRACTOR_H
