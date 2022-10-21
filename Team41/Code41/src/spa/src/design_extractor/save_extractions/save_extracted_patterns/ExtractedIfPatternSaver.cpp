//
// Created by Aaron on 12/10/22.
//

#include "ExtractedIfPatternSaver.h"

#include "design_extractor/PatternExtractor.h"

using namespace DE;

void ExtractedIfPatternSaver::save(shared_ptr<ProgramNode> programNode,
                                   shared_ptr<DataModifier> dataModifier) {
  auto ifPatternList = PatternExtractor::extractIfPattern(programNode);
  for (auto entry : ifPatternList) {
    dataModifier->saveIfPattern(entry);
  }
}
