//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_USESSRELATIONEXTRACTOR_H
#define SPA_USESSRELATIONEXTRACTOR_H
#include "AST/CondExprNode.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/UsesRelationExtractor.h"

namespace DE {

class UsesSRelationExtractor : public UsesRelationExtractor {
  unordered_set<string> extractUsesSHelper(
      shared_ptr<ASTNode> node, shared_ptr<list<vector<string>>> result);

  void insertCallsInIfAndWhileForUseS();
  void insertCallsForUseS();

  void initIfAndWhileStmtNoToUsedVarsMap();
  StrToSetMap ifWhileStmtNoToUsedVarsMap;

 public:
  UsesSRelationExtractor(shared_ptr<DataModifier> dataModifier,
                         shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_USESSRELATIONEXTRACTOR_H
