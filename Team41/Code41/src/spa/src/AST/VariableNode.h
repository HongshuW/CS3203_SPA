//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

#include <string>

#include "ASTNode.h"

using namespace std;

namespace AST {
class VariableNode : public ASTNode {
 public:
  string variable;
  explicit VariableNode(string variable);

  bool operator==(const ASTNode& node) const override;
};
}  // namespace AST

#endif  // SPA_VARIABLENODE_H
