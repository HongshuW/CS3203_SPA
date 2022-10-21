//
// Created by Aaron on 12/10/22.
//

#include "ExtractedFollowsSaver.h"

#include "design_extractor/FollowsExtractor.h"

void ExtractedFollowsSaver::save(shared_ptr<AST::ProgramNode> programNode,
                                 shared_ptr<DataModifier> dataModifier) {
  list<vector<string>> followsList =
      *FollowsExtractor().extractFollows(programNode);
  list<vector<string>> followsStarList =
      *FollowsExtractor().extractFollowsStar(programNode);

  for (auto entry : followsList) {
    dataModifier->saveFollows(entry);
  }

  for (auto entry : followsStarList) {
    dataModifier->saveFollowsT(entry);
  }
}
