//
// Created by Xingchen Lin on 26/10/22.
//

#ifndef SPA_CALLSRELATIONEXTRACTOR_H
#define SPA_CALLSRELATIONEXTRACTOR_H

#include "AST/CallNode.h"
#include "AST/utils/ASTUtils.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/EntitiesExtractor.h"
#include "design_extractor/results/RelationResult.h"
#include "design_extractor/utils/DesignExtractorUtils.h"

namespace DE {

class CallsRelationExtractor : public AbstractDesignExtractor {
 private:
  shared_ptr<list<vector<string>>> extractCalls(
      const shared_ptr<ProgramNode>& programNode);

 public:
  CallsRelationExtractor(shared_ptr<DataModifier> dataModifier,
                         shared_ptr<ProgramNode> programNode);
  ExtractorResult extract() override;
  void save(ExtractorResult result) override;
};

}  // namespace DE

#endif  // SPA_CALLSRELATIONEXTRACTOR_H
