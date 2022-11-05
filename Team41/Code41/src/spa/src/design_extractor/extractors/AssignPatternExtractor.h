//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_ASSIGNPATTERNEXTRACTOR_H
#define SPA_ASSIGNPATTERNEXTRACTOR_H
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/results/AssignPatternResult.h"

namespace DE {

class AssignPatternExtractor : public AbstractDesignExtractor {
  void extractorHelper(const shared_ptr<ASTNode>& node);
  AssignResult output;

 public:
  AssignPatternExtractor(shared_ptr<DataModifier> dataModifier,
                         shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_ASSIGNPATTERNEXTRACTOR_H
