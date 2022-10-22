//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_PKBSAVERINTERFACE_H
#define SPA_PKBSAVERINTERFACE_H

#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"

class PKBSaverInterface {
 public:
  PKBSaverInterface(shared_ptr<ProgramNode> programNode,
                    shared_ptr<DataModifier> dataModifier);
  virtual void save(shared_ptr<ProgramNode> programNode,
                    shared_ptr<DataModifier> dataModifier) = 0;

 private:
  shared_ptr<ProgramNode> programNode;
  shared_ptr<DataModifier> dataModifier;
};

#endif  // SPA_PKBSAVERINTERFACE_H
