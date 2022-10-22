//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_PATTERNEXTRACTOR_H
#define SPA_PATTERNEXTRACTOR_H

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "AST/AssignNode.h"
#include "AST/CondExprNode.h"
#include "AST/ProgramNode.h"
namespace DE {
class PatternExtractor {
 private:
  static void extractPatternHelper(
      shared_ptr<ASTNode> node,
      shared_ptr<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>>
          resultVec,
      shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers);
  static unordered_set<string> condExprNodeHandler(
      shared_ptr<CondExprNode> condExpr);
  static void relExprNodeHandler(shared_ptr<RelExprNode> relExpr,
                                 unordered_set<string>& varList);
  static void exprNodeHandler(shared_ptr<ExprNode> expr,
                              unordered_set<string>& varList);
  static void getRelExprNodesDFS(
      shared_ptr<CondExprNode> condExpr,
      vector<shared_ptr<RelExprNode>>& relExprNodeList);
  static void getVarFromExprNodesDFS(shared_ptr<ExprNode> expr,
                                     unordered_set<string>& varList);

 public:
  static vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>
  extractPattern(shared_ptr<ProgramNode> programNode);
  static list<vector<string>> extractIfPattern(
      shared_ptr<ProgramNode> programNode);
  static list<vector<string>> extractWhilePattern(
      shared_ptr<ProgramNode> programNode);
};

}  // namespace DE

#endif  // SPA_PATTERNEXTRACTOR_H
