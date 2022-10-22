//
// Created by Tan Wei En on 3/9/22.
//

#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "AST/ProgramNode.h"
#include "AffectsExtractor.h"
#include "NextExtractor.h"
#include "args/StmtNoArgs.h"
#include "pkb/DataModifier.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/RelationType.h"

using namespace QB;
namespace DE {
class DesignExtractor {
 private:
  shared_ptr<ProgramNode> programNode;
  shared_ptr<DataModifier> dataModifier;

 public:
  explicit DesignExtractor(shared_ptr<DataModifier> dataModifier,
                           shared_ptr<ProgramNode> programNode);
  void run();

  vector<string> getNextStarRelations(StmtNoArgs args);
  vector<string> getAffectsRelations(StmtNoArgs args);
  vector<string> getAffectsStarRelations(StmtNoArgs args);

  list<vector<string>> getAllNextStarRelations();
  list<vector<string>> getAllAffectsRelations();
  list<vector<string>> getAllAffectsStarRelations();
};
}  // namespace DE

#endif  // SPA_DESIGNEXTRACTOR_H
