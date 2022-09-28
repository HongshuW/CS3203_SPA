//
// Created by Xingchen Lin on 28/9/22.
//

#include "AssignPatternClause.h"

#include <utility>

AssignPatternClause::AssignPatternClause(Synonym arg1, Ref arg2, ExpressionSpec arg3) : PatternClause(arg1, arg2),
        arg1(std::move(arg1)), arg2(std::move(arg2)), arg3(std::move(arg3)) {}

bool AssignPatternClause::operator==(const AssignPatternClause &clause) const {
    auto patternClause = dynamic_cast<const AssignPatternClause*>(&clause);
    return patternClause != nullptr && arg1 == patternClause->arg1
        && arg2 == patternClause->arg2 && arg3 == patternClause->arg3;
}