//
// Created by Tan Wei En on 4/10/22.
//

#ifndef SPA_INVALIDPROCEDUREDUMMIES_H
#define SPA_INVALIDPROCEDUREDUMMIES_H

#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"

namespace TestAST {

class InvalidProcedureDummies {
 public:
  static shared_ptr<ProgramNode> getTestProgramNode(int idx);
};

}  // namespace TestAST

#endif  // SPA_INVALIDPROCEDUREDUMMIES_H
