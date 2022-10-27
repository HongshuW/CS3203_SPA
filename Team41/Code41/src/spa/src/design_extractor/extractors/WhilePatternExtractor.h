//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_WHILEPATTERNEXTRACTOR_H
#define SPA_WHILEPATTERNEXTRACTOR_H
#include "design_extractor/extractors/IfWhilePatternExtractor.h"
namespace DE {

class WhilePatternExtractor : public IfWhilePatternExtractor {
 public:
  WhilePatternExtractor(shared_ptr<DataModifier> dataModifier,
                        shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_WHILEPATTERNEXTRACTOR_H
