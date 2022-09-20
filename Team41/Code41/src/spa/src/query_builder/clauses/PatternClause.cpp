//
// Created by Xingchen Lin on 11/9/22.
//

#include "PatternClause.h"

#include <utility>

PatternClause::PatternClause(DesignEntity patternType, Synonym arg1, Ref arg2, ExpressionSpec arg3) :
        patternType(patternType), arg1(std::move(arg1)), arg2(std::move(arg2)), arg3(arg3) {};

PatternClause::PatternClause(DesignEntity patternType, Synonym arg1, Ref arg2) :
        patternType(patternType), arg1(std::move(arg1)), arg2(std::move(arg2)) {};