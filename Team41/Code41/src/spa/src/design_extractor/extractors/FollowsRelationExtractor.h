//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_FOLLOWSRELATIONEXTRACTOR_H
#define SPA_FOLLOWSRELATIONEXTRACTOR_H
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/results/RelationResult.h"

namespace DE {

class FollowsRelationExtractor : public AbstractDesignExtractor {
  void initListOfStmtList();
  void extractHelper(const vector<shared_ptr<StmtNode>>& stmtList);
  vector<vector<shared_ptr<StmtNode>>> listOfStmtList;
  shared_ptr<list<vector<string>>> output;

 public:
  FollowsRelationExtractor(shared_ptr<DataModifier> dataModifier,
                           shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_FOLLOWSRELATIONEXTRACTOR_H
