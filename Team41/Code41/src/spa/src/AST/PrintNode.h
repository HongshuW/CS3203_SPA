//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H

#include "ASTNode.h"
#include "StmtNode.h"
#include "VariableNode.h"

using namespace std;

namespace AST {
class PrintNode : public StmtNode {
 public:
  shared_ptr<VariableNode> variableNode;

  explicit PrintNode(shared_ptr<VariableNode> variableNode);

  bool operator==(const ASTNode& node) const override;
};
}  // namespace AST

#endif  // SPA_PRINTNODE_H
