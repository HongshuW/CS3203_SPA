//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_READNODE_H
#define SPA_READNODE_H

#include "ASTNode.h"
#include "StmtNode.h"
#include "VariableNode.h"

using namespace std;
using namespace AST;

namespace AST {
class ReadNode : public StmtNode {
 public:
  shared_ptr<VariableNode> variableNode;

  explicit ReadNode(shared_ptr<VariableNode> variableNode);

  bool operator==(const ASTNode& node) const override;
};
}  // namespace AST

#endif  // SPA_READNODE_H
