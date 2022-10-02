//
// Created by Xingchen Lin on 28/9/22.
//

#include "PatternClause.h"

#include <utility>
#include "AssignPatternClause.h"
#include "IfPatternClause.h"
#include "WhilePatternClause.h"

PatternClause::PatternClause(Synonym arg1, Ref arg2) : arg1(std::move(arg1)), arg2(std::move(arg2)) {}

unsigned int PatternClause::validateSyntaxError(int currIdx, const vector<string>& tokens) {
    return -1;
}

bool PatternClause::expect(const string& s, unsigned int currIdx, const vector<string>& tokens) {
    if (currIdx < tokens.size() && tokens[currIdx] == s) {
        return true;
    }
    string errorMessage = ErrorMessageFormatter::formatErrorMessage(s, tokens[currIdx]);
    throw PQLParseException(errorMessage);
}
