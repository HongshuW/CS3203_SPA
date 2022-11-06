//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_QUERYTIMEDESIGNEXTRACTOR_H
#define SPA_QUERYTIMEDESIGNEXTRACTOR_H

#include "AST/ProgramNode.h"
#include "AST/utils/ASTUtils.h"
#include "args/StmtNoArgs.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/results/ExtractorResult.h"
#include "pkb/DataModifier.h"

namespace DE {

class QueryTimeDesignExtractor {
 public:
  explicit QueryTimeDesignExtractor(const shared_ptr<ProgramNode>& programNode);
  virtual shared_ptr<ExtractorResult> extract(StmtNoArgs args) = 0;
  virtual shared_ptr<ExtractorResult> extractAllRelations() = 0;
  bool isArgsWithinRange(StmtNoArgs args);
 protected:
  shared_ptr<ProgramNode> programNode;
  shared_ptr<StmtNumbers> stmtNumbers;
};

}  // namespace DE

#endif  // SPA_QUERYTIMEDESIGNEXTRACTOR_H
