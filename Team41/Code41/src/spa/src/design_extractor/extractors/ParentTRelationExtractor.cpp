//
// Created by Xingchen Lin on 27/10/22.
//

#include "ParentTRelationExtractor.h"

namespace DE {
ParentTRelationExtractor::ParentTRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : ParentCommonExtractor(std::move(dataModifier), std::move(programNode)) {}

void ParentTRelationExtractor::extractParentTDFS(
    const shared_ptr<map<int, vector<int>>>& parentRelations, int key,
    vector<string> ancestors, const shared_ptr<list<vector<string>>>& output) {
  if (parentRelations->count(key) == 0) {
    return;
  }
  // add current stmt to ancestors
  ancestors.push_back(to_string(key));
  // pop current stmt's children
  vector<int> children = parentRelations->at(key);
  parentRelations->erase(key);
  for (const string& ancestor : ancestors) {
    for (int child : children) {
      // add combinations to outputs
      vector<string> row{ancestor, to_string(child)};
      output->push_back(row);
      // dfs
      extractParentTDFS(parentRelations, child, ancestors, output);
    }
  }
}

shared_ptr<ExtractorResult> ParentTRelationExtractor::extract() {
  shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();
  shared_ptr<map<int, vector<int>>> parentRelations =
      extractParentHashmap(programNode);
  vector<string> ancestors;

  while (!parentRelations->empty()) {
    auto iterator = parentRelations->begin();
    int key = iterator->first;
    extractParentTDFS(parentRelations, key, ancestors, output);
  }

  return make_shared<RelationResult>(output);
}

void ParentTRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> parentTResult =
      static_pointer_cast<RelationResult>(result);
  for (const auto& entry : *parentTResult->getResult()) {
    dataModifier->saveParent(entry);
  }
}
}  // namespace DE