//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_ENTITIESEXTRACTOR_H
#define SPA_ENTITIESEXTRACTOR_H
#include "AST/CallNode.h"
#include "AST/CondExprNode.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/results/ExtractorResult.h"

namespace DE {

class EntitiesExtractor : public AbstractDesignExtractor {
  shared_ptr<unordered_set<string>> extractVariables(
      shared_ptr<StmtNode> stmtNode);
  shared_ptr<unordered_set<string>> extractConstants(
      shared_ptr<StmtNode> stmtNode);
  vector<shared_ptr<StmtNode>> extractStmtNodes();
  shared_ptr<unordered_set<string>> extractAllVariables();
  shared_ptr<unordered_set<string>> extractAllConstants();
  shared_ptr<unordered_set<string>> extractAllProcedures();
  unordered_set<string> getVariablesFromExprNode(shared_ptr<ExprNode> exprNode);
  unordered_set<string> getConstantsFromExprNode(shared_ptr<ExprNode> exprNode);
  unordered_set<string> getVariablesFromCondExprNode(
      shared_ptr<CondExprNode> condExprNode);
  unordered_set<string> getConstantsFromCondExprNode(
      shared_ptr<CondExprNode> condExprNode);

  vector<shared_ptr<StmtNode>> stmtNodeList;

 public:
  EntitiesExtractor(shared_ptr<DataModifier> dataModifier,
                    shared_ptr<ProgramNode> programNode);

  ExtractorResult extract() override;

 protected:
  void save(ExtractorResult result) override;
};

}  // namespace DE

#endif  // SPA_ENTITIESEXTRACTOR_H
