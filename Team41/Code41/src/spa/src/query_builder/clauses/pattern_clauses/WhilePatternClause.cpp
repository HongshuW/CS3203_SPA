//
// Created by Xingchen Lin on 28/9/22.
//

#include "WhilePatternClause.h"

WhilePatternClause::WhilePatternClause(const Synonym& arg1, const Ref& arg2)
    : PatternClause(arg1, arg2) {}

bool WhilePatternClause::operator==(const WhilePatternClause& other) const {
  auto clause = dynamic_cast<const WhilePatternClause*>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table WhilePatternClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

void WhilePatternClause::validateSyntaxError() {
}

