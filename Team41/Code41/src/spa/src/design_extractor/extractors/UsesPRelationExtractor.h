//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_USESPRELATIONEXTRACTOR_H
#define SPA_USESPRELATIONEXTRACTOR_H
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/UsesRelationExtractor.h"

namespace DE {

class UsesPRelationExtractor : public UsesRelationExtractor {
 public:
  UsesPRelationExtractor(shared_ptr<DataModifier> dataModifier,
                         shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_USESPRELATIONEXTRACTOR_H
