//
// Created by Nafour on 29/10/22.
//

#include "AffectsCommonExtractor.h"

#include <utility>

namespace DE {
AffectsCommonExtractor::AffectsCommonExtractor(
    shared_ptr<ProgramNode> programNode)
    : QueryTimeDesignExtractor(std::move(programNode)) {
  programSize = 0;
  offset = 0;
  currCFG = nullptr;
  initialize();
}

void AffectsCommonExtractor::initialize() {
  lineNoToNodePtrMap = ASTUtils::getLineNumToNodePtrMap(programNode);
  procCFGMap = generateProcCFGMap(programNode, stmtNumbers);
  stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);
  procToModifiedVars = initProcedureToModifiedVarMap(programNode);
}

//! DFS the CFG nodes
void AffectsCommonExtractor::DFS(int curr,
                                 const shared_ptr<vector<int>>& visitCount,
                                 unordered_map<string, int> lastModifiedMap) {
  int currNode = curr - offset;
  visitCount->at(currNode)++;
  auto nodePtr = lineNoToNodePtrMap->at(curr);
  auto nodeType = ASTUtils::getNodeType(nodePtr);

  if (nodeType == AST::ASSIGN_NODE) {
    //! If the node has been used, update the affectsTable
    shared_ptr<AssignNode> assignNode =
        static_pointer_cast<AssignNode>(nodePtr);
    unordered_set<string> usedVariables =
        getVariablesFromExprNode(assignNode->expressionNode);
    for (const auto& variable : usedVariables) {
      if (!lastModifiedMap.count(variable)) continue;
      //! Make pair and insert to affectsTable
      vector<string> affectRelation = {to_string(lastModifiedMap.at(variable)),
                                       to_string(curr)};
      affectsTable->insert(affectRelation);
    }

    //! If the has been modified, update the lastModifiedMap
    string modifiedVariable = assignNode->variableNode->variable;
    if (lastModifiedMap.count(modifiedVariable)) {
      lastModifiedMap.erase(modifiedVariable);
    }
    lastModifiedMap.insert({{modifiedVariable, curr}});
  }

  if (nodeType == AST::CALL_NODE) {
    shared_ptr<CallNode> callNode = static_pointer_cast<CallNode>(nodePtr);
    shared_ptr<unordered_set<string>> modifiedVars =
        procToModifiedVars->at(callNode->procedureName);
    //! Clear entry in lastModifiedMap
    for (auto& var : *modifiedVars) {
      lastModifiedMap.erase(var);
    }
  }

  if (nodeType == AST::READ_NODE) {
    shared_ptr<ReadNode> readNode = static_pointer_cast<ReadNode>(nodePtr);
    string modifiedVar = readNode->variableNode->variable;
    //! Clear entry in lastModifiedMap
    lastModifiedMap.erase(modifiedVar);
  }

  unordered_set<int> children = currCFG->cfg->find(curr)->second;
  for (auto child : children) {
    if (visitCount->at(child - offset) >= 2) continue;
    DFS(child, visitCount, lastModifiedMap);
    visitCount->at(child - offset)--;
  }
}

void AffectsCommonExtractor::generateAffectsTable() {
  affectsTable = make_shared<set<vector<string>>>();
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  FirstLineNoToProcMap firstLineNumToProcMap =
      ASTUtils::getFirstLineNumToProcMap(programNode);
  for (const shared_ptr<ProcedureNode>& procedure : procedureList) {
    int currLineNumber = firstLineNumToProcMap->at(procedure);
    CFG cfg = procCFGMap->at(procedure);
    int visitedSize = (int)cfg.cfg->size();
    shared_ptr<vector<int>> visitCount =
        make_shared<vector<int>>(visitedSize, 0);
    offset = currLineNumber;
    currCFG = make_shared<CFG>(cfg);
    DFS(currLineNumber, visitCount, unordered_map<string, int>());
  }
}

shared_ptr<unordered_map<string, shared_ptr<unordered_set<string>>>>
AffectsCommonExtractor::initProcedureToModifiedVarMap(
    shared_ptr<ProgramNode> programNode) {
  StrToSetMap proceduresToModifiedVarsMap =
      make_shared<unordered_map<string, shared_ptr<unordered_set<string>>>>();
  for (const auto& procedureNode : programNode->procedureList) {
    string procedureName = procedureNode->procedureName;
    shared_ptr<unordered_set<string>> modifiedVariables =
        getModifiedVariablesFromProcedure(procedureNode);
    proceduresToModifiedVarsMap->insert({procedureName, modifiedVariables});
  }
  auto resultMap =
      make_shared<unordered_map<string, shared_ptr<unordered_set<string>>>>();

  auto mappedCallNodesToProcedures =
      extractCallNodesFromProcedures(programNode);
  for (const auto& pair : *proceduresToModifiedVarsMap) {
    unordered_set<string> uniqueVarList;
    string procedureName = pair.first;
    auto currentUsedVarList = pair.second;
    for (const auto& v : *currentUsedVarList) {
      uniqueVarList.insert(v);
    }

    // if the procedures has call nodes, handle them
    if (mappedCallNodesToProcedures.count(procedureName) != 0) {
      auto callNodes = mappedCallNodesToProcedures.at(procedureName);

      for (const auto& node : callNodes) {
        extractVariablesFromCallNodesInProceduresToList(
            node, proceduresToModifiedVarsMap, mappedCallNodesToProcedures,
            uniqueVarList);
      }
    }

    resultMap->insert(
        {procedureName, make_shared<unordered_set<string>>(uniqueVarList)});
  }
  return resultMap;
}

void AffectsCommonExtractor::clearCache() { affectsTable = nullptr; }

}  // namespace DE