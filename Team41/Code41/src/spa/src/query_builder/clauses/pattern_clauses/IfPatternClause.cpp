//
// Created by Xingchen Lin on 28/9/22.
//

#include "IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_evaluator/IVisitor.h"

IfPatternClause::IfPatternClause(Synonym arg1, Ref arg2) : PatternClause(arg1, arg2),
        arg1(std::move(arg1)), arg2(std::move(arg2)) {}

bool IfPatternClause::operator==(const IfPatternClause &clause) const {
    auto patternClause = dynamic_cast<const IfPatternClause*>(&clause);
    return patternClause != nullptr && arg1 == patternClause->arg1
           && arg2 == patternClause->arg2;
}

Clause IfPatternClause::asClauseVariant() {
    return shared_from_this();
}

Table IfPatternClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}
