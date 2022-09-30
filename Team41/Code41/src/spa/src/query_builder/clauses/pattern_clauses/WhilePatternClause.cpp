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

Clause WhilePatternClause::asClauseVariant() {
    return shared_from_this();
}

Table WhilePatternClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}
