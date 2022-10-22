//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

#include <vector>

#include "ASTNode.h"
#include "AssignNode.h"

using namespace std;
using namespace AST;

namespace AST {
class ProcedureNode : public ASTNode {
 public:
  string procedureName;
  vector<shared_ptr<StmtNode>> stmtList;

  ProcedureNode(string procedureName, vector<shared_ptr<StmtNode>> stmtList);

  bool operator==(const ASTNode& node) const override;
};
}  // namespace AST

#endif  // SPA_PROCEDURENODE_H
