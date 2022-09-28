//
// Created by Xingchen Lin on 11/9/22.
//

#include "PatternClause.h"

#include <utility>

PatternClause::PatternClause(DesignEntity patternType, Synonym arg1, Ref arg2, ExpressionSpec arg3) :
        patternType(patternType), arg1(std::move(arg1)), arg2(std::move(arg2)), arg3(arg3) {}

PatternClause::PatternClause(DesignEntity patternType, Synonym arg1, Ref arg2) :
        patternType(patternType), arg1(std::move(arg1)), arg2(std::move(arg2)) {}

bool PatternClause::operator==(const Clause &clause) const {
    auto patternClause = dynamic_cast<const PatternClause*>(&clause);
    return patternClause != nullptr && patternType == patternClause->patternType &&
            arg1 == patternClause->arg1 && arg2 == patternClause->arg2 && arg3 == patternClause->arg3;
}

ostream& PatternClause::print(ostream & os) const {
    os << "Pattern Clause";
    return os;
}

Table PatternClause::accept(shared_ptr<QE::ClauseEvaluator> clauseEvaluator) {

}