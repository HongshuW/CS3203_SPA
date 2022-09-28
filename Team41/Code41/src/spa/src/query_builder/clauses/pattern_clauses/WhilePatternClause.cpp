//
// Created by Xingchen Lin on 28/9/22.
//

#include "WhilePatternClause.h"

WhilePatternClause::WhilePatternClause(const Synonym& arg1, const Ref& arg2) : PatternClause(arg1, arg2), arg1(arg1), arg2(arg2) {};

bool WhilePatternClause::operator==(const WhilePatternClause &clause) const {
    auto patternClause = dynamic_cast<const WhilePatternClause*>(&clause);
    return patternClause != nullptr && arg1 == patternClause->arg1
           && arg2 == patternClause->arg2;
}