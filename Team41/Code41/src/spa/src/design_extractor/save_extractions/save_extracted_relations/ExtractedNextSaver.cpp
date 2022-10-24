//
// Created by Aaron on 12/10/22.
//

#include "ExtractedNextSaver.h"

#include "design_extractor/NextExtractor.h"

void ExtractedNextSaver::save(shared_ptr<ProgramNode> programNode,
                              shared_ptr<DataModifier> dataModifier) {
  list<vector<string>> nextList = *NextExtractor::extractNext();
  for (auto entry : nextList) {
    dataModifier->saveNext(entry);
  }
}
