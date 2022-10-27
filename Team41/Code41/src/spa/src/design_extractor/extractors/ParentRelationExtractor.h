//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_PARENTRELATIONEXTRACTOR_H
#define SPA_PARENTRELATIONEXTRACTOR_H

#include <map>

#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/results/RelationResult.h"
#include "design_extractor/utils/ParentExtractorUtils.h"

namespace DE {

class ParentRelationExtractor : public AbstractDesignExtractor,
                                public ParentExtractorUtils {
 public:
  ParentRelationExtractor(shared_ptr<DataModifier> dataModifier,
                          shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_PARENTRELATIONEXTRACTOR_H
