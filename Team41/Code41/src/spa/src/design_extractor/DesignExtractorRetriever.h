//
// Created by Xingchen Lin on 28/10/22.
//

#ifndef SPA_DESIGNEXTRACTORRETRIEVER_H
#define SPA_DESIGNEXTRACTORRETRIEVER_H

#include <list>
#include <string>
#include <vector>

#include "AST/ProgramNode.h"
#include "args/StmtNoArgs.h"
#include "design_extractor/extractors/NextRelationExtractor.h"

using namespace std;

namespace DE {

class DesignExtractorRetriever {
 private:
  shared_ptr<ProgramNode> programNode;

 public:
  explicit DesignExtractorRetriever(shared_ptr<ProgramNode> programNode);
  //! For query time computation clauses
  //! PKB will call these functions
  shared_ptr<vector<string>> getNextStarRelations(StmtNoArgs args);
  shared_ptr<vector<string>> getAffectsRelations(StmtNoArgs args);
  shared_ptr<vector<string>> getAffectsStarRelations(StmtNoArgs args);
  shared_ptr<list<vector<string>>> getAllNextStarRelations();
  shared_ptr<list<vector<string>>> getAllAffectsRelations();
  shared_ptr<list<vector<string>>> getAllAffectsStarRelations();
};

}  // namespace DE

#endif  // SPA_DESIGNEXTRACTORRETRIEVER_H
