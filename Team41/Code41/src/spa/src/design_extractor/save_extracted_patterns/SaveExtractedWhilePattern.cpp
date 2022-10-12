//
// Created by Aaron on 12/10/22.
//

#include "SaveExtractedWhilePattern.h"
#include "../PatternExtractor.h"

using namespace DE;

void SaveExtractedWhilePattern::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    auto whilePatternList = PatternExtractor::extractWhilePattern(programNode);
    for (auto entry: whilePatternList) {
        dataModifier->saveWhilePattern(entry);
    }
}
