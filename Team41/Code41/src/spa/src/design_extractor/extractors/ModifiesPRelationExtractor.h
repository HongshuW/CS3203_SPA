//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_MODIFIESPRELATIONEXTRACTOR_H
#define SPA_MODIFIESPRELATIONEXTRACTOR_H
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/ModifiesRelationExtractor.h"
#include "design_extractor/utils/DesignExtractorUtils.h"

namespace DE {

class ModifiesPRelationExtractor : public ModifiesRelationExtractor {
 public:
  ModifiesPRelationExtractor(shared_ptr<DataModifier> dataModifier,
                             shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_MODIFIESPRELATIONEXTRACTOR_H
