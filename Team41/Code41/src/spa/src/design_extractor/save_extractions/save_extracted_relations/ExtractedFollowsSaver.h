//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_EXTRACTEDFOLLOWSSAVER_H
#define SPA_EXTRACTEDFOLLOWSSAVER_H

#include "AST/ProgramNode.h"
#include "design_extractor/save_extractions/save_to_pkb/PKBSaverInterface.h"
#include "pkb/DataModifier.h"

class ExtractedFollowsSaver : public PKBSaverInterface {
  ;
  void save(shared_ptr<ProgramNode> programNode,
            shared_ptr<DataModifier> dataModifier) override;

 public:
  ExtractedFollowsSaver(shared_ptr<ProgramNode> programNode,
                        shared_ptr<DataModifier> dataModifier)
      : PKBSaverInterface(std::move(programNode), std::move(dataModifier)) {}
};

#endif  // SPA_EXTRACTEDFOLLOWSSAVER_H
