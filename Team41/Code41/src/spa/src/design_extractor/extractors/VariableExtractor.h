//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_VARIABLEEXTRACTOR_H
#define SPA_VARIABLEEXTRACTOR_H

#include "AST/CondExprNode.h"
#include "AST/ProgramNode.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/EntitiesExtractor.h"
#include "design_extractor/extractors/interfaces/VariableExtractionInterface.h"
#include "design_extractor/results/ExtractorResult.h"
#include "pkb/DataModifier.h"
namespace DE {

class VariableExtractor : public EntitiesExtractor,
                          public VariableExtractionInterface {
  shared_ptr<unordered_set<string>> extractVariables(
      const shared_ptr<StmtNode>& stmtNode);

 public:
  VariableExtractor(shared_ptr<DataModifier> dataModifier,
                    shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_VARIABLEEXTRACTOR_H
