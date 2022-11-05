//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_IFWHILEPATTERNEXTRACTOR_H
#define SPA_IFWHILEPATTERNEXTRACTOR_H
#include "AST/CondExprNode.h"
#include "design_extractor/AbstractDesignExtractor.h"

namespace DE {
class IfWhilePatternExtractor : public AbstractDesignExtractor {
 protected:
  IfWhilePatternExtractor(shared_ptr<DataModifier> dataModifier,
                          shared_ptr<ProgramNode> programNode);
  shared_ptr<list<vector<string>>> output;

  unordered_set<string> condExprNodeHandler(const shared_ptr<CondExprNode>& condExpr);

 private:
  void relExprNodeHandler(const shared_ptr<RelExprNode>& relExpr,
                          unordered_set<string>& varList);
  void exprNodeHandler(const shared_ptr<ExprNode>& expr,
                       unordered_set<string>& varList);
  void getRelExprNodesDFS(const shared_ptr<CondExprNode>& condExpr,
                          vector<shared_ptr<RelExprNode>>& relExprNodeList);

  void getVarFromExprNodesDFS(const shared_ptr<ExprNode>& expr,
                              unordered_set<string>& varList);
};
}  // namespace DE

#endif  // SPA_IFWHILEPATTERNEXTRACTOR_H
