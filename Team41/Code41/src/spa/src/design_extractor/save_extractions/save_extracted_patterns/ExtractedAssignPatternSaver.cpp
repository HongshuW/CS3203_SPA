//
// Created by Aaron on 12/10/22.
//

#include "ExtractedAssignPatternSaver.h"
#include "design_extractor/PatternExtractor.h"

using namespace DE;

void ExtractedAssignPatternSaver::save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) {
    auto resultList = PatternExtractor::extractPattern(programNode);
    for (auto resultRow: resultList) {
        string lineNumStr = to_string(resultRow.first.first);
        string varName = resultRow.first.second;
        auto exprNode = resultRow.second->expressionNode;
        dataModifier->saveAssignPattern({lineNumStr, varName}, exprNode);
    }
}