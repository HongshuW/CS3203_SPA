//
// Created by hongshu wang on 12/9/22.
//

#include "PatternManager.h"

PatternTable PatternManager::patternTable = PatternTable();

shared_ptr<Table> PatternManager::getMatchedPatterns(ExpressionSpec expressionSpec) {
    return patternTable.getMatchedPatterns(expressionSpec);
}

void PatternManager::savePattern(vector<string> metainfo, shared_ptr<ExprNode> pattern) {
    patternTable.addPattern(metainfo, pattern);
}
