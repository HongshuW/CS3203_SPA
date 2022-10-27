//
// Created by Xingchen Lin on 27/10/22.
//

#include "ParentCommonExtractor.h"

namespace DE {
ParentCommonExtractor::ParentCommonExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {
}

shared_ptr<map<int, vector<int>>> ParentCommonExtractor::extractParentHashmap(
    shared_ptr<ProgramNode> rootPtr) {
  map<int, vector<int>> parentRelations;
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(rootPtr);
  queue<shared_ptr<ASTNode>> queue;
  queue.push(rootPtr);

  while (!queue.empty()) {
    shared_ptr<ASTNode> current = queue.front();
    queue.pop();

    NodeType nodeType = ASTUtils::getNodeType(current);
    switch (nodeType) {
      case AST::WHILE_NODE: {
        shared_ptr<WhileNode> ptr = dynamic_pointer_cast<WhileNode>(current);
        vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
        int parent = stmtNumbers->at(ptr);
        vector<int> children;
        for (shared_ptr<StmtNode> n : stmtList) {
          queue.push(n);
          int child = stmtNumbers->at(n);
          children.push_back(child);
        }
        pair<int, vector<int>> row{parent, children};
        parentRelations.insert(row);
        break;
      }
      case AST::IF_NODE: {
        shared_ptr<IfNode> ptr = dynamic_pointer_cast<IfNode>(current);
        vector<shared_ptr<StmtNode>> ifStmtList = ptr->ifStmtList;
        vector<shared_ptr<StmtNode>> elseStmtList = ptr->elseStmtList;
        int parent = stmtNumbers->at(ptr);
        vector<int> children;
        for (shared_ptr<StmtNode> n : ifStmtList) {
          queue.push(n);
          int child = stmtNumbers->at(n);
          children.push_back(child);
        }
        for (shared_ptr<StmtNode> n : elseStmtList) {
          queue.push(n);
          int child = stmtNumbers->at(n);
          children.push_back(child);
        }
        pair<int, vector<int>> row{parent, children};
        parentRelations.insert(row);
        break;
      }
      case AST::PROGRAM_NODE: {
        // encounter a program node, check its procedures
        shared_ptr<ProgramNode> ptr =
            dynamic_pointer_cast<ProgramNode>(current);
        vector<shared_ptr<ProcedureNode>> procedureList = ptr->procedureList;
        for (shared_ptr<ProcedureNode> n : procedureList) {
          queue.push(n);
        }
        break;
      }
      case AST::PROCEDURE_NODE: {
        // encounter a procedure node, check its statements
        shared_ptr<ProcedureNode> ptr =
            dynamic_pointer_cast<ProcedureNode>(current);
        vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
        for (shared_ptr<StmtNode> n : stmtList) {
          queue.push(n);
        }
        break;
      }
      default:
        break;
    }
  }

  return make_shared<map<int, vector<int>>>(parentRelations);
}
}  // namespace DE