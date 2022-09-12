//
// Created by hongshu wang on 12/9/22.
//

#include "PatternManager.h"

PatternTable PatternManager::patternTable = PatternTable();

shared_ptr<Table> PatternManager::getMatchedPatterns(ExpressionSpec expressionSpec) {
    return patternTable.getMatchedPatterns(expressionSpec);
}
