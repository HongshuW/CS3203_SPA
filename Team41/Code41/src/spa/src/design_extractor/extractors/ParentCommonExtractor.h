//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_PARENTCOMMONEXTRACTOR_H
#define SPA_PARENTCOMMONEXTRACTOR_H

#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "AST/ProgramNode.h"
#include "AST/utils/ASTUtils.h"
#include "design_extractor/AbstractDesignExtractor.h"

using namespace std;
namespace DE {

class ParentCommonExtractor : public AbstractDesignExtractor {
 protected:
  ParentCommonExtractor(shared_ptr<DataModifier> dataModifier,
                        shared_ptr<ProgramNode> programNode);
  //! Only child components can access this method
  shared_ptr<map<int, vector<int>>> extractParentHashmap(
      shared_ptr<ProgramNode> rootPtr);
};

}  // namespace DE

#endif  // SPA_PARENTCOMMONEXTRACTOR_H
