//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_NEXTRELATIONEXTRACTOR_H
#define SPA_NEXTRELATIONEXTRACTOR_H

#include "CFG/CFG.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/args/StmtNoArgs.h"
#include "design_extractor/results/RelationResult.h"
#include "design_extractor/utils/DesignExtractorUtils.h"

namespace DE {
typedef shared_ptr<unordered_map<shared_ptr<ProcedureNode>, int>>
    FirstLineNoToProcMap;

class NextRelationExtractor : AbstractDesignExtractor {
 public:
  NextRelationExtractor(shared_ptr<DataModifier> dataModifier,
                        shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_NEXTRELATIONEXTRACTOR_H
