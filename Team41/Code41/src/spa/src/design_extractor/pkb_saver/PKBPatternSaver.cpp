//
// Created by Aaron on 8/10/22.
//

#include "PKBPatternSaver.h"
#include "../PatternExtractor.h"


using namespace DE;

void PKBPatternSaver::saveIfPattern(shared_ptr<ProgramNode> programNode,
                                    shared_ptr<DataModifier> dataModifier) {
    list<vector<string>> ifPatternList = PatternExtractor::extractIfPattern(programNode);
    for (auto entry: ifPatternList) {
        dataModifier->saveIfPattern(entry);
    }
}

void PKBPatternSaver::saveWhilePattern(shared_ptr<ProgramNode> programNode,
                                       shared_ptr<DataModifier> dataModifier) {
    list<vector<string>> whilePatternList = PatternExtractor::extractWhilePattern(programNode);
    for (auto entry: whilePatternList) {
        dataModifier->saveWhilePattern(entry);
    }
}