//
// Created by Nafour on 9/9/22.
//

#ifndef SPA_DUMMIES_H
#define SPA_DUMMIES_H
#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
namespace TestDE {

class Dummies {
 public:
  static shared_ptr<ProgramNode> getTestProgramNode(int idx);
};

}  // namespace TestDE

#endif  // SPA_DUMMIES_H
