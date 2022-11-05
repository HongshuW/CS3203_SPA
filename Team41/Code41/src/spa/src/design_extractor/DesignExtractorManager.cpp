//
// Created by Xingchen Lin on 28/10/22.
//

#include "DesignExtractorManager.h"

#include <utility>

namespace DE {
DesignExtractorManager::DesignExtractorManager(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : dataModifier(std::move(dataModifier)), programNode(std::move(programNode)) {}

void DesignExtractorManager::run() {
  // save statement lines and types
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> nodeToLine =
      ASTUtils::getNodePtrToLineNumMap(programNode);
  list<vector<string>> payload;
  auto it = payload.begin();
  for (std::pair<shared_ptr<StmtNode>, int> myPair : *nodeToLine) {
    shared_ptr<StmtNode> node = myPair.first;
    vector<string> stmtPair = {
        to_string(myPair.second),
        getDesignEntityString(ASTUtils::getStmtNodeDesignEntity(node))};
    it = payload.insert(it, stmtPair);
  }
  this->dataModifier->saveStatements(payload);

  // save relations and patterns and entities
  saveToPKB();
}

void DesignExtractorManager::saveToPKB() {
  saveEntitiesToPKB();
  savePatternsToPKB();
  saveRelationsToPKB();
}

void DesignExtractorManager::saveEntitiesToPKB() {
  vector<shared_ptr<AbstractDesignExtractor>> entitiesExtractors = {
      make_shared<ConstantExtractor>(dataModifier, programNode),
      make_shared<VariableExtractor>(dataModifier, programNode),
      make_shared<ProcedureExtractor>(dataModifier, programNode)};

  for (const auto& extractor : entitiesExtractors) {
    extractor->save(extractor->extract());
  }
}

void DesignExtractorManager::savePatternsToPKB() {
  vector<shared_ptr<AbstractDesignExtractor>> patternsExtractors = {
      make_shared<AssignPatternExtractor>(dataModifier, programNode),
      make_shared<IfPatternExtractor>(dataModifier, programNode),
      make_shared<WhilePatternExtractor>(dataModifier, programNode)};

  for (const auto& extractor : patternsExtractors) {
    extractor->save(extractor->extract());
  }
}

void DesignExtractorManager::saveRelationsToPKB() {
  //! Save all relations except Next*, Affects and Affects*
  vector<shared_ptr<AbstractDesignExtractor>> relationsExtractors = {
      make_shared<ParentRelationExtractor>(dataModifier, programNode),
      make_shared<ParentTRelationExtractor>(dataModifier, programNode),
      make_shared<FollowsRelationExtractor>(dataModifier, programNode),
      make_shared<FollowsTRelationExtractor>(dataModifier, programNode),
      make_shared<UsesPRelationExtractor>(dataModifier, programNode),
      make_shared<UsesSRelationExtractor>(dataModifier, programNode),
      make_shared<ModifiesPRelationExtractor>(dataModifier, programNode),
      make_shared<ModifiesSRelationExtractor>(dataModifier, programNode),
      make_shared<CallsRelationExtractor>(dataModifier, programNode),
      make_shared<CallsTRelationExtractor>(dataModifier, programNode),
      make_shared<NextRelationExtractor>(dataModifier, programNode)};

  for (const auto& extractor : relationsExtractors) {
    extractor->save(extractor->extract());
  }
}
}  // namespace DE