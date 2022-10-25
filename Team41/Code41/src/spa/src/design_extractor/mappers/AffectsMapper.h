//
// Created by Aaron on 20/10/22.
//

#ifndef SPA_AFFECTSMAPPER_H
#define SPA_AFFECTSMAPPER_H

#include <unordered_map>
#include <unordered_set>

#include "AST/ProgramNode.h"

using namespace std;

class AffectsMapper {
 public:
  AffectsMapper(shared_ptr<ProgramNode> programNode);

  unordered_map<int, unordered_set<int>> getValidStartToEndArgsMap();
  unordered_map<int, unordered_set<int>> getValidEndToStartArgsMap();
  unordered_map<int, unordered_set<string>> getModifiedVarMap();
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> getNodePtrToLineNumMap();
  shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>>
  getLineNumToProcMap();
  unordered_map<string, shared_ptr<StmtNode>>
  getStmtNoOfAssignReadAndCallNodesMap();
  unordered_map<string, shared_ptr<StmtNode>> getStmtNoOfAssignNodesMap();

 private:
  shared_ptr<ProgramNode> programNode;
  unordered_map<int, unordered_set<int>> validStartToEndArgsMap;
  unordered_map<int, unordered_set<int>> validEndToStartArgsMap;
  unordered_map<int, unordered_set<string>> modifiedVarMap;
  unordered_map<string, shared_ptr<StmtNode>> stmtNoToAssignReadAndCallNodesMap;
  unordered_map<string, shared_ptr<StmtNode>> stmtNoToAssignNodesMap;
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> nodePtrToLineNumMap;
  shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> lineNumToProcMap;

  unordered_map<string, shared_ptr<StmtNode>> mapStmtNoToAssignReadAndCallNodes(
      shared_ptr<ProgramNode> programNode);

  void initValidArgsMap();
  void initModifiedVarMap();
  void initStmtNoToAssignReadAndCallNodesMap();
  void initStmtNoToAssignNodesMap();
  void initNodePtrToLineNumMap();
  void initLineNumToProcMap();
};

#endif  // SPA_AFFECTSMAPPER_H
