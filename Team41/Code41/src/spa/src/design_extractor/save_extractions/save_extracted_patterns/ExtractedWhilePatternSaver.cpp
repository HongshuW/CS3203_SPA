//
// Created by Aaron on 12/10/22.
//

#include "ExtractedWhilePatternSaver.h"

#include "design_extractor/PatternExtractor.h"

using namespace DE;

void ExtractedWhilePatternSaver::save(shared_ptr<ProgramNode> programNode,
                                      shared_ptr<DataModifier> dataModifier) {
  auto whilePatternList = PatternExtractor::extractWhilePattern(programNode);
  for (auto entry : whilePatternList) {
    dataModifier->saveWhilePattern(entry);
  }
}
