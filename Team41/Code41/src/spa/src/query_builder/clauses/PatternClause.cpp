//
// Created by Xingchen Lin on 11/9/22.
//

#include "PatternClause.h"

PatternClause::PatternClause(Synonym arg1, Ref arg2, ExpressionSpec arg3) :
arg1(arg1), arg2(arg2), arg3(arg3) {};

PatternClause::PatternClause(QB::Synonym arg1, QB::Ref arg2) : arg1(arg1), arg2(arg2) {};