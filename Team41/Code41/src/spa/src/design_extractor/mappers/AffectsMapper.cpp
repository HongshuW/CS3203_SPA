//
// Created by Aaron on 20/10/22.
//

#include "AffectsMapper.h"

#include <queue>

#include "../EntityExtractor.h"
#include "../ModifiesExtractor.h"
#include "AST/utils/ASTUtils.h"

AffectsMapper::AffectsMapper(shared_ptr<ProgramNode> programNode) {
  this->programNode = programNode;
  initValidArgsMap();
  initModifiedVarMap();
  initStmtNoToAssignReadAndCallNodesMap();
  initNodePtrToLineNumMap();
  initLineNumToProcMap();
  initStmtNoToAssignNodesMap();
}

void AffectsMapper::initValidArgsMap() {
  shared_ptr<unordered_map<shared_ptr<ProcedureNode>, unordered_set<int>>>
      procToLineNumMap = ASTUtils::getProcToLineNumMap(programNode);
  auto stmtNoToAssignReadAndCallMap =
      mapStmtNoToAssignReadAndCallNodes(programNode);
  for (const auto &[proc, lineNumList] : *procToLineNumMap) {
    unordered_set<int> tempLineNumList;
    // filter only stmt no that are assign nodes
    for (int lineNum : lineNumList) {
      bool stmtNoIsAssignReadOrCall =
          stmtNoToAssignReadAndCallMap.count(to_string(lineNum)) != 0;
      if (stmtNoIsAssignReadOrCall) {
        auto node = stmtNoToAssignReadAndCallMap.at(to_string(lineNum));
        NodeType nodeType = ASTUtils::getNodeType(node);
        bool isStmtNodeAssign = nodeType == AST::ASSIGN_NODE;
        if (isStmtNodeAssign) {
          tempLineNumList.insert(lineNum);
        }
      }
    }

    // check if variable modified in start assign node
    // is used in end assign node
    for (int start : tempLineNumList) {
      unordered_set<int> finalLineNumList = tempLineNumList;
      for (int end : tempLineNumList) {
        auto startNode = dynamic_pointer_cast<AssignNode>(
            stmtNoToAssignReadAndCallMap.at(to_string(start)));
        auto endNode = dynamic_pointer_cast<AssignNode>(
            stmtNoToAssignReadAndCallMap.at(to_string(end)));
        string modifiedVar = startNode->variableNode->variable;
        unordered_set<string> usedVars =
            DE::EntityExtractor::getVariablesFromExprNode(
                endNode->expressionNode);
        if (usedVars.count(modifiedVar) == 0) {
          finalLineNumList.erase(end);
        }
      }
      validStartToEndArgsMap.insert(make_pair(start, finalLineNumList));
    }

    // check if variables used in end assign node
    // is modified in start assign node
    for (int end : tempLineNumList) {
      unordered_set<int> finalLineNumList = tempLineNumList;
      for (int start : tempLineNumList) {
        auto endNode = dynamic_pointer_cast<AssignNode>(
            stmtNoToAssignReadAndCallMap.at(to_string(end)));
        auto startNode = dynamic_pointer_cast<AssignNode>(
            stmtNoToAssignReadAndCallMap.at(to_string(start)));
        string modifiedVar = startNode->variableNode->variable;
        unordered_set<string> usedVars =
            DE::EntityExtractor::getVariablesFromExprNode(
                endNode->expressionNode);
        if (usedVars.count(modifiedVar) == 0) {
          finalLineNumList.erase(start);
        }
      }
      validEndToStartArgsMap.insert(make_pair(end, finalLineNumList));
    }
  }
}

void AffectsMapper::initModifiedVarMap() {
  auto stmtNoToAssignReadAndCallMap =
      mapStmtNoToAssignReadAndCallNodes(programNode);
  auto mapProcToModifiedVar =
      DE::ModifiesExtractor::mapProceduresToModifiedVariables(programNode);
  for (const auto &[stmtNo, stmtNode] : stmtNoToAssignReadAndCallMap) {
    NodeType nodeType = ASTUtils::getNodeType(stmtNode);
    switch (nodeType) {
      case AST::ASSIGN_NODE: {
        shared_ptr<AssignNode> assignNode =
            dynamic_pointer_cast<AssignNode>(stmtNode);
        string modifiedVar = assignNode->variableNode->variable;
        int num = stoi(stmtNo);
        unordered_set<string> varList = {modifiedVar};
        modifiedVarMap.insert(make_pair(num, varList));
        break;
      }

      case AST::READ_NODE: {
        shared_ptr<ReadNode> readNode =
            dynamic_pointer_cast<ReadNode>(stmtNode);
        string modifiedVar = readNode->variableNode->variable;
        int num = stoi(stmtNo);
        unordered_set<string> varList = {modifiedVar};
        modifiedVarMap.insert(make_pair(num, varList));
        break;
      }
      case AST::CALL_NODE: {
        shared_ptr<CallNode> callNode =
            dynamic_pointer_cast<CallNode>(stmtNode);
        if (mapProcToModifiedVar.count(callNode->procedureName) != 0) {
          auto modifiedVarListFromProc =
              mapProcToModifiedVar.at(callNode->procedureName);
          int num = stoi(stmtNo);
          unordered_set<string> varList = {};
          for (const auto &v : modifiedVarListFromProc) {
            varList.insert(v);
          }
          modifiedVarMap.insert(make_pair(num, varList));
        }
        break;
      }
      default:
        break;
    }
  }
}

void AffectsMapper::initStmtNoToAssignReadAndCallNodesMap() {
  stmtNoToAssignReadAndCallNodesMap =
      mapStmtNoToAssignReadAndCallNodes(programNode);
}

void AffectsMapper::initStmtNoToAssignNodesMap() {
  auto outputMap = unordered_map<string, shared_ptr<StmtNode>>();
  auto stmtNoToAssignReadAndCallNodes =
      mapStmtNoToAssignReadAndCallNodes(programNode);
  for (const auto &[stmtNo, stmtNode] : stmtNoToAssignReadAndCallNodes) {
    NodeType nodeType = ASTUtils::getNodeType(stmtNode);
    if (nodeType == AST::ASSIGN_NODE) {
      outputMap.insert(make_pair(stmtNo, stmtNode));
    }
  }
  stmtNoToAssignNodesMap = outputMap;
}

void AffectsMapper::initNodePtrToLineNumMap() {
  nodePtrToLineNumMap = ASTUtils::getNodePtrToLineNumMap(programNode);
}

void AffectsMapper::initLineNumToProcMap() {
  lineNumToProcMap = ASTUtils::getLineNumToProcMap(programNode);
}

unordered_map<string, shared_ptr<StmtNode>>
AffectsMapper::mapStmtNoToAssignReadAndCallNodes(
    shared_ptr<ProgramNode> programNode) {
  auto outputMap = unordered_map<string, shared_ptr<StmtNode>>();
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(programNode);
  for (auto procedureNode : programNode->procedureList) {
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while (!queue.empty()) {
      auto stmtList = queue.front();
      queue.pop();
      for (auto stmtNode : stmtList) {
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
          case AST::IF_NODE: {
            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
            vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
            vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
            queue.push(ifStmtList);
            queue.push(elseStmtList);
            break;
          }
          case AST::WHILE_NODE: {
            shared_ptr<WhileNode> whileNode =
                dynamic_pointer_cast<WhileNode>(stmtNode);
            vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
            queue.push(whileStmtList);
            break;
          }

          case AST::ASSIGN_NODE: {
            shared_ptr<AssignNode> assignNode =
                dynamic_pointer_cast<AssignNode>(stmtNode);
            int stmtNo = stmtNumbers->at(assignNode);
            outputMap.insert({to_string(stmtNo), stmtNode});
            break;
          }

          case AST::READ_NODE: {
            shared_ptr<ReadNode> readNode =
                dynamic_pointer_cast<ReadNode>(stmtNode);
            int stmtNo = stmtNumbers->at(readNode);
            outputMap.insert({to_string(stmtNo), stmtNode});
            break;
          }
          case AST::CALL_NODE: {
            shared_ptr<CallNode> callNode =
                dynamic_pointer_cast<CallNode>(stmtNode);
            int stmtNo = stmtNumbers->at(callNode);
            outputMap.insert({to_string(stmtNo), stmtNode});
            break;
          }
          default:
            break;
        }
      }
    }
  }

  return outputMap;
}

unordered_map<int, unordered_set<int>>
AffectsMapper::getValidStartToEndArgsMap() {
  return validStartToEndArgsMap;
}

unordered_map<int, unordered_set<int>>
AffectsMapper::getValidEndToStartArgsMap() {
  return validEndToStartArgsMap;
}

unordered_map<int, unordered_set<string>> AffectsMapper::getModifiedVarMap() {
  return modifiedVarMap;
}

shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>
AffectsMapper::getNodePtrToLineNumMap() {
  return nodePtrToLineNumMap;
}

shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>>
AffectsMapper::getLineNumToProcMap() {
  return lineNumToProcMap;
}

unordered_map<string, shared_ptr<StmtNode>>
AffectsMapper::getStmtNoOfAssignReadAndCallNodesMap() {
  return stmtNoToAssignReadAndCallNodesMap;
}

unordered_map<string, shared_ptr<StmtNode>>
AffectsMapper::getStmtNoOfAssignNodesMap() {
  return stmtNoToAssignNodesMap;
}