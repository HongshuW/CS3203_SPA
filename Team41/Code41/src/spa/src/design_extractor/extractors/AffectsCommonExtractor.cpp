//
// Created by Nafour on 29/10/22.
//

#include "AffectsCommonExtractor.h"

namespace DE {
AffectsCommonExtractor::AffectsCommonExtractor(
    shared_ptr<ProgramNode> programNode)
    : QueryTimeDesignExtractor(programNode) {
  offset = 0;
  currCFG = nullptr;
  initialize();
}

void AffectsCommonExtractor::initialize() {
  lineNoToNodePtrMap = ASTUtils::getLineNumToNodePtrMap(programNode);
  procCFGMap = generateProcCFGMap(programNode, stmtNumbers);
  stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);

  // generate modifiesP map
  modifiesPMap =
      make_shared<unordered_map<string, shared_ptr<unordered_set<string>>>>(
          unordered_map<string, shared_ptr<unordered_set<string>>>());
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  for (const shared_ptr<ProcedureNode>& procedure : procedureList) {
    modifiesPMap->insert({procedure->procedureName,
                          getModifiedVariablesFromProcedure(procedure)});
  }
}

//! DFS the CFG nodes
void AffectsCommonExtractor::DFS(int curr, shared_ptr<vector<int>> visitCount,
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
        modifiesPMap->at(callNode->procedureName);
    //! Clear entry in lastModifiedMap
    for (auto& var : *modifiedVars) {
      lastModifiedMap.erase(var);
    }
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
    int visitedSize = cfg.cfg->size();
    shared_ptr<vector<int>> visitCount =
        make_shared<vector<int>>(visitedSize, 0);
    offset = currLineNumber;
    currCFG = make_shared<CFG>(cfg);
    DFS(currLineNumber, visitCount, unordered_map<string, int>());
  }
}

void AffectsCommonExtractor::clearCache() { affectsTable = nullptr; }
}  // namespace DE