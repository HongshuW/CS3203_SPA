//
// Created by Tan Wei En on 3/9/22.
//

#ifndef SPA_ASSIGNNODE_H
#define SPA_ASSIGNNODE_H

#include "ASTNode.h"
#include "ExprNode.h"
#include "StmtNode.h"
#include "VariableNode.h"

using namespace std;
using namespace AST;

namespace AST {
class AssignNode : public StmtNode {
 public:
  shared_ptr<VariableNode> variableNode;
  shared_ptr<ExprNode> expressionNode;

  AssignNode(shared_ptr<VariableNode> variableNode,
             shared_ptr<ExprNode> expressionNode);

  bool operator==(const ASTNode& node) const override;
};
}  // namespace AST

#endif  // SPA_ASSIGNNODE_H
