//
// Created by Aaron on 12/10/22.
//

#ifndef SPA_PKBSAVEREXECUTOR_H
#define SPA_PKBSAVEREXECUTOR_H

#include <vector>

#include "AST/ProgramNode.h"
#include "PKBSaverInterface.h"
#include "pkb/DataModifier.h"

class PKBSaverExecutor {
 public:
  PKBSaverExecutor(shared_ptr<ProgramNode> programNode,
                   shared_ptr<DataModifier> dataModifier);
  void executeSave();

 private:
  void initRelations(
      vector<shared_ptr<PKBSaverInterface>>& extractedDesignList);
  void initPatterns(vector<shared_ptr<PKBSaverInterface>>& extractedDesignList);
  void initEntities(vector<shared_ptr<PKBSaverInterface>>& extractedDesignList);

  shared_ptr<ProgramNode> programNode;
  shared_ptr<DataModifier> dataModifier;
};

#endif  // SPA_PKBSAVEREXECUTOR_H
