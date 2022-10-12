//
// Created by Aaron on 12/10/22.
//

#include "SaveExtractedAssignPattern.h"
#include "../PatternExtractor.h"

using namespace DE;

void SaveExtractedAssignPattern::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    auto resultList = PatternExtractor::extractPattern(programNode);
    for (auto resultRow: resultList) {
        string lineNumStr = to_string(resultRow.first.first);
        string varName = resultRow.first.second;
        auto exprNode = resultRow.second->expressionNode;
        dataModifier->saveAssignPattern({lineNumStr, varName}, exprNode);
    }
}
