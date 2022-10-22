//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_CONDEXPRNODE_H
#define SPA_CONDEXPRNODE_H

#include <string>

#include "ASTNode.h"
#include "RelExprNode.h"

using namespace std;

namespace AST {

class CondExprNode : public ASTNode {
 public:
  shared_ptr<RelExprNode> relExprNode = nullptr;
  shared_ptr<CondExprNode> condExprLHS = nullptr;
  string op = "";
  shared_ptr<CondExprNode> condExprRHS = nullptr;

  // x == y
  explicit CondExprNode(shared_ptr<RelExprNode> relExprNode);

  // ! (x == y)
  explicit CondExprNode(shared_ptr<CondExprNode> condExprLHS);

  // (x == y) && (z == 1)
  CondExprNode(shared_ptr<CondExprNode> condExprLHS, string op,
               shared_ptr<CondExprNode> condExprRHS);

  bool operator==(const ASTNode& node) const override;
};

}  // namespace AST

#endif  // SPA_CONDEXPRNODE_H
