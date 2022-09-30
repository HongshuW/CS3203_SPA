//
// Created by Xingchen Lin on 28/9/22.
//

#include "PatternClause.h"

#include <utility>
#include "AssignPatternClause.h"
#include "IfPatternClause.h"
#include "WhilePatternClause.h"

PatternClause::PatternClause(Synonym arg1, Ref arg2) : arg1(std::move(arg1)), arg2(std::move(arg2)) {}

//Table PatternClause::accept(shared_ptr<IVisitor> visitor) {
//    visitor->visit(shared_from_this());
//    return Table();
//}

