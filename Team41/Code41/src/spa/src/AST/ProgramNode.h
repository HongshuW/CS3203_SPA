//
// Created by Tan Wei En on 3/9/22.
//

#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

#include <vector>

#include "ASTNode.h"
#include "ProcedureNode.h"

using namespace std;

namespace AST {
class ProgramNode : public ASTNode {
 public:
  vector<shared_ptr<ProcedureNode>> procedureList;

  explicit ProgramNode(vector<shared_ptr<ProcedureNode>> procedureList);

  bool operator==(const ASTNode& node) const override;
};
}  // namespace AST

#endif  // SPA_PROGRAMNODE_H
