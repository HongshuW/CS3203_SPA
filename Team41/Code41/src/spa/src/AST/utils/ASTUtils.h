//
// Created by Nafour on 9/9/22.
//

#ifndef SPA_ASTUTILS_H
#define SPA_ASTUTILS_H

#include <unordered_map>
#include <unordered_set>

#include "AST/ASTNode.h"
#include "AST/AssignNode.h"
#include "AST/CallNode.h"
#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "AST/ProcedureNode.h"
#include "AST/ProgramNode.h"
#include "AST/ReadNode.h"
#include "AST/WhileNode.h"
#include "query_builder/commons/DesignEntity.h"

using namespace QB;
namespace AST {
enum NodeType {
  ASSIGN_NODE,
  CALL_NODE,
  IF_NODE,
  PRINT_NODE,
  READ_NODE,
  WHILE_NODE,
  CONDEXPR_NODE,
  EXPR_NODE,
  PROCEDURE_NODE,
  PROGRAM_NODE,
  VARIABLE_NODE
};

typedef shared_ptr<unordered_map<shared_ptr<ProcedureNode>, int>>
    FirstLineNoToProcMap;

class ASTUtils {
  static int getNodeLineMappingHelper(
      const shared_ptr<ASTNode>& node,
      const shared_ptr<unordered_map<shared_ptr<ASTNode>, int>>& map, int curr_line_no,
      const shared_ptr<unordered_map<int, shared_ptr<ASTNode>>>& lineToNode);
  static int getNodePtrToLineNoMapHelper(
      const shared_ptr<ASTNode>& node,
      const shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>& map,
      int curr_line_no);

 public:
  static NodeType getNodeType(const shared_ptr<ASTNode>& node);
  static void getNodeLineMapping(
      const shared_ptr<ProgramNode>& root,
      const shared_ptr<unordered_map<shared_ptr<ASTNode>, int>>& nodeToLine,
      const shared_ptr<unordered_map<int, shared_ptr<ASTNode>>>& lineToNode);
  static shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>
  getNodePtrToLineNumMap(const shared_ptr<ProgramNode>& root);
  static shared_ptr<unordered_map<int, shared_ptr<StmtNode>>>
  getLineNumToNodePtrMap(const shared_ptr<ProgramNode>& root);
  static DesignEntity getStmtNodeDesignEntity(const shared_ptr<StmtNode>& node);
  static shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>>
  getLineNumToProcMap(const shared_ptr<ProgramNode>& root);
  static shared_ptr<unordered_map<shared_ptr<ProcedureNode>, int>>
  getFirstLineNumToProcMap(const shared_ptr<ProgramNode>& root);
};

}  // namespace AST

#endif  // SPA_ASTUTILS_H
