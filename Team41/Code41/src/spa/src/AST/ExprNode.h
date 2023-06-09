//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_EXPRNODE_H
#define SPA_EXPRNODE_H

#include <string>

#include "ASTNode.h"
#include "utils/Utils.h"

using namespace AST;
using namespace std;

namespace AST {

class ExprNode : public ASTNode {
 public:
  string expr;
  shared_ptr<ExprNode> left;
  shared_ptr<ExprNode> right;

  explicit ExprNode(string expr);

  [[nodiscard]] bool isVariableNode() const;
  [[nodiscard]] bool isOperatorNode() const;

  bool operator==(const ASTNode& node) const override;
};

}  // namespace AST

#endif  // SPA_EXPRNODE_H
