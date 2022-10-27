//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_PARENTEXTRACTORUTILS_H
#define SPA_PARENTEXTRACTORUTILS_H

#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "AST/ProgramNode.h"
#include "AST/utils/ASTUtils.h"

using namespace std;
namespace DE {

class ParentExtractorUtils {
 protected:
  //! Only child components can access this method
  shared_ptr<map<int, vector<int>>> extractParentHashmap(
      shared_ptr<ProgramNode> rootPtr);
};

}  // namespace DE

#endif  // SPA_PARENTEXTRACTORUTILS_H
