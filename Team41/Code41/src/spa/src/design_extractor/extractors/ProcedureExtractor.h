//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_PROCEDUREEXTRACTOR_H
#define SPA_PROCEDUREEXTRACTOR_H
#include "AST/ProgramNode.h"
#include "design_extractor/extractors/EntitiesExtractor.h"
#include "design_extractor/results/ExtractorResult.h"
#include "pkb/DataModifier.h"

namespace DE {

class ProcedureExtractor : public EntitiesExtractor {
 public:
  ProcedureExtractor(shared_ptr<DataModifier> dataModifier,
                     shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_PROCEDUREEXTRACTOR_H
