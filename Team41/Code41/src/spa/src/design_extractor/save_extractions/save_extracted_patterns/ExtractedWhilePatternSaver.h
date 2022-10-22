//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_EXTRACTEDWHILEPATTERNSAVER_H
#define SPA_EXTRACTEDWHILEPATTERNSAVER_H

#include "design_extractor/save_extractions/save_to_pkb/PKBSaverInterface.h"

class ExtractedWhilePatternSaver : public PKBSaverInterface {
 public:
  ExtractedWhilePatternSaver(shared_ptr<ProgramNode> programNode,
                             shared_ptr<DataModifier> dataModifier)
      : PKBSaverInterface(std::move(programNode), std::move(dataModifier)) {}

  void save(shared_ptr<ProgramNode> programNode,
            shared_ptr<DataModifier> dataModifier) override;
};

#endif  // SPA_EXTRACTEDWHILEPATTERNSAVER_H
