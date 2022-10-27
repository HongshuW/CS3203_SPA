//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_FOLLOWSTRELATIONEXTRACTOR_H
#define SPA_FOLLOWSTRELATIONEXTRACTOR_H
#include "design_extractor/AbstractDesignExtractor.h"

namespace DE {

class FollowsTRelationExtractor : AbstractDesignExtractor {
  void initListOfStmtList();
  void extractHelper(vector<shared_ptr<StmtNode>> stmtList);
  vector<vector<shared_ptr<StmtNode>>> listOfStmtList;
  shared_ptr<list<vector<string>>> output;

 public:
  FollowsTRelationExtractor(shared_ptr<DataModifier> dataModifier,
                            shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_FOLLOWSTRELATIONEXTRACTOR_H
