//
// Created by Xingchen Lin on 27/10/22.
//

#include "ParentRelationExtractor.h"

namespace DE {
ParentRelationExtractor::ParentRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {
}

shared_ptr<ExtractorResult> ParentRelationExtractor::extract() {
  shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();
  shared_ptr<map<int, vector<int>>> parentRelations =
      extractParentHashmap(programNode);
  auto hashmapIterator = parentRelations->begin();
  while (hashmapIterator != parentRelations->end()) {
    string parent = to_string(hashmapIterator->first);
    vector<int> children = hashmapIterator->second;
    for (int child : children) {
      vector<string> row{parent, to_string(child)};
      output->push_back(row);
    }
    advance(hashmapIterator, 1);
  }

  return make_shared<RelationResult>(output);
}

void ParentRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> parentResult =
      static_pointer_cast<RelationResult>(result);
  for (auto entry : *parentResult->getResult()) {
    dataModifier->saveParent(entry);
  }
}
}  // namespace DE