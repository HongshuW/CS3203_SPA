//
// Created by Xingchen Lin on 26/10/22.
//

#ifndef SPA_CALLSTRELATIONEXTRACTOR_H
#define SPA_CALLSTRELATIONEXTRACTOR_H

#include "AST/CallNode.h"
#include "AST/utils/ASTUtils.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/EntitiesExtractor.h"
#include "design_extractor/extractors/interfaces/CallNodeExtractionInterface.h"
#include "design_extractor/results/RelationResult.h"

namespace DE {

class CallsTRelationExtractor : public AbstractDesignExtractor,
                                public CallNodeExtractionInterface {
 public:
  CallsTRelationExtractor(shared_ptr<DataModifier> dataModifier,
                          shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_CALLSTRELATIONEXTRACTOR_H
