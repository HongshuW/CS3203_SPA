//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_EXTRACTEDPARENTSSAVER_H
#define SPA_EXTRACTEDPARENTSSAVER_H

#include "AST/ProgramNode.h"
#include "design_extractor/save_extractions/save_to_pkb/PKBSaverInterface.h"
#include "pkb/DataModifier.h"

class ExtractedParentsSaver : public PKBSaverInterface {
 public:
  ExtractedParentsSaver(shared_ptr<ProgramNode> programNode,
                        shared_ptr<DataModifier> dataModifier)
      : PKBSaverInterface(std::move(programNode), std::move(dataModifier)){};
  void save(shared_ptr<ProgramNode> programNode,
            shared_ptr<DataModifier> dataModifier) override;
};

#endif  // SPA_EXTRACTEDPARENTSSAVER_H
