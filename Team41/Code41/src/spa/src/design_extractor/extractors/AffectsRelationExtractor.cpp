//
// Created by Xingchen Lin on 28/10/22.
//

#include "AffectsRelationExtractor.h"

#include "NextRelationExtractor.h"

namespace DE {
AffectsRelationExtractor::AffectsRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : QueryTimeDesignExtractor(std::move(dataModifier),
                               std::move(programNode)) {
  initialize();
}

void AffectsRelationExtractor::initialize() {
  lineNoToNodePtrMap = ASTUtils::getLineNumToNodePtrMap(programNode);
  procCFGMap = generateProcCFGMap(programNode, stmtNumbers);
  stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);
  affectsTable = make_shared<set<vector<string>>>();

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
void AffectsRelationExtractor::DFS(int curr, CFG cfg,
                                   shared_ptr<vector<int>> visitCount,
                                   int offset,
                                   unordered_map<string, int> lastModifiedMap) {
  int currNode = curr - offset;
  if (visitCount->at(currNode) >= 2) return;
  visitCount->at(currNode)++;
  auto nodePtr = lineNoToNodePtrMap->at(curr);
  auto nodeType = ASTUtils::getNodeType(nodePtr);

  if (nodeType == AST::ASSIGN_NODE) {
    //! If the node has been used, update the affectsTable
    shared_ptr<AssignNode> assignNode =
        static_pointer_cast<AssignNode>(nodePtr);
    unordered_set<string> usedVariables =
        getVariablesFromExprNode(assignNode->expressionNode);
    for (auto variable : usedVariables) {
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

  unordered_set<int> children = cfg.cfg->find(curr)->second;
  for (auto child : children) {
    DFS(child, cfg, visitCount, offset, lastModifiedMap);
    visitCount->at(child - offset)--;
  }
}

shared_ptr<ExtractorResult> AffectsRelationExtractor::extract(StmtNoArgs args) {

}

shared_ptr<ExtractorResult> AffectsRelationExtractor::extractAllRelations() {
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  FirstLineNoToProcMap firstLineNumToProcMap =
      ASTUtils::getFirstLineNumToProcMap(programNode);
  for (const shared_ptr<ProcedureNode>& procedure : procedureList) {
    int currLineNumber = firstLineNumToProcMap->at(procedure);
    CFG currCFG = procCFGMap->at(procedure);
    int visitedSize = currCFG.cfg->size();
    shared_ptr<vector<int>> visitCount =
        make_shared<vector<int>>(visitedSize, 0);
    DFS(currLineNumber, currCFG, visitCount, currLineNumber,
        unordered_map<string, int>());
  }

  shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();
  for (auto& entry : *affectsTable) {
    output->push_back(entry);
  }

  return make_shared<RelationResult>(output);
}
}  // namespace DE