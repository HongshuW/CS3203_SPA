//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_CONSTANTEXTRACTOR_H
#define SPA_CONSTANTEXTRACTOR_H
#include "design_extractor/extractors/EntitiesExtractor.h"

namespace DE {

class ConstantExtractor : public EntitiesExtractor {
  shared_ptr<unordered_set<string>> extractConstants(
      const shared_ptr<StmtNode>& stmtNode);
  unordered_set<string> getConstantsFromExprNode(
      const shared_ptr<ExprNode>& exprNode);
  unordered_set<string> getConstantsFromCondExprNode(
      const shared_ptr<CondExprNode>& condExprNode);

 public:
  ConstantExtractor(shared_ptr<DataModifier> dataModifier,
                    shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_CONSTANTEXTRACTOR_H
