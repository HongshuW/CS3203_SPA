//
// Created by Xingchen Lin on 28/9/22.
//

#include "PatternClause.h"

#include <utility>

PatternClause::PatternClause(Synonym arg1, Ref arg2) : arg1(std::move(arg1)), arg2(std::move(arg2)) {}