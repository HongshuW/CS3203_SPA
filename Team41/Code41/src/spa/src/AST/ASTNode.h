//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_ASTNODE_H
#define SPA_ASTNODE_H

#include <memory>

using namespace std;

namespace AST {
class ASTNode {
 public:
  virtual ~ASTNode() = default;
  virtual bool operator==(const ASTNode& node) const = 0;
};
}  // namespace AST

#endif  // SPA_ASTNODE_H
