//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_ENTITIESEXTRACTOR_H
#define SPA_ENTITIESEXTRACTOR_H
#include "AST/CallNode.h"
#include "AST/CondExprNode.h"
#include "design_extractor/AbstractDesignExtractor.h"

namespace DE {

class EntitiesExtractor : public AbstractDesignExtractor {
  shared_ptr<unordered_set<string>> extractVariables(
      shared_ptr<StmtNode> stmtNode);
  shared_ptr<unordered_set<string>> extractConstants(
      shared_ptr<StmtNode> stmtNode);
  shared_ptr<unordered_set<string>> extractAllVariables(
      shared_ptr<ProgramNode> rootPtr);
  shared_ptr<unordered_set<string>> extractAllConstants(
      shared_ptr<ProgramNode> rootPtr);
  shared_ptr<unordered_set<string>> extractAllProcedures(
      shared_ptr<ProgramNode> rootPtr);
  unordered_set<string> getVariablesFromExprNode(shared_ptr<ExprNode> exprNode);
  unordered_set<string> getConstantsFromExprNode(shared_ptr<ExprNode> exprNode);
  unordered_set<string> getVariablesFromCondExprNode(
      shared_ptr<CondExprNode> condExprNode);
  unordered_set<string> getConstantsFromCondExprNode(
      shared_ptr<CondExprNode> condExprNode);

 public:
  EntitiesExtractor(shared_ptr<DataModifier> dataModifier,
                    shared_ptr<ProgramNode> programNode);
  void extract() override;

 protected:
  void save() override;
};

}  // namespace DE

#endif  // SPA_ENTITIESEXTRACTOR_H
