//
// Created by Aaron on 14/10/22.
//

#ifndef SPA_EXTRACTEDVARIABLESAVER_H
#define SPA_EXTRACTEDVARIABLESAVER_H

#include "../save_to_pkb/PKBSaverInterface.h"

class ExtractedVariableSaver : public PKBSaverInterface {
 public:
  ExtractedVariableSaver(shared_ptr<ProgramNode> programNode,
                         shared_ptr<DataModifier> dataModifier)
      : PKBSaverInterface(std::move(programNode), std::move(dataModifier)) {}

  void save(shared_ptr<ProgramNode> programNode,
            shared_ptr<DataModifier> dataModifier) override;
};

#endif  // SPA_EXTRACTEDVARIABLESAVER_H
