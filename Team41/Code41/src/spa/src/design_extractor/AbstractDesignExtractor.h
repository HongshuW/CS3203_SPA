//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_ABSTRACTDESIGNEXTRACTOR_H
#define SPA_ABSTRACTDESIGNEXTRACTOR_H
#include "AST/ProgramNode.h"
#include "design_extractor/results/ExtractorResult.h"
#include "pkb/DataModifier.h"

namespace DE {
typedef unordered_map<shared_ptr<StmtNode>, int> StmtNumbers;

class AbstractDesignExtractor {
 public:
  explicit AbstractDesignExtractor(shared_ptr<DataModifier> dataModifier,
                                   shared_ptr<ProgramNode> programNode);
  virtual shared_ptr<ExtractorResult> extract() = 0;
  virtual void save(shared_ptr<ExtractorResult> result) = 0;

 protected:
  shared_ptr<DataModifier> dataModifier;
  shared_ptr<ProgramNode> programNode;
  shared_ptr<StmtNumbers> stmtNumbers;
};

}  // namespace DE

#endif  // SPA_ABSTRACTDESIGNEXTRACTOR_H
