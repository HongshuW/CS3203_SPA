//
// Created by Xingchen Lin on 28/9/22.
//

#include "IfPatternClause.h"

IfPatternClause::IfPatternClause(Synonym arg1, Ref arg2) : PatternClause(arg1, arg2),
        arg1(std::move(arg1)), arg2(std::move(arg2)) {}

bool IfPatternClause::operator==(const IfPatternClause &clause) const {
    auto patternClause = dynamic_cast<const IfPatternClause*>(&clause);
    return patternClause != nullptr && arg1 == patternClause->arg1
           && arg2 == patternClause->arg2;
}