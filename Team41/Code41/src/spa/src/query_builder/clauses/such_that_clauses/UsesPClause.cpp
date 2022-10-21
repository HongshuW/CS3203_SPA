//
// Created by Xingchen Lin on 30/9/22.
//

#include "UsesPClause.h"

UsesPClause::UsesPClause(Ref arg1, Ref arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool UsesPClause::operator==(const UsesPClause &other) const {
  auto clause = dynamic_cast<const UsesPClause *>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table UsesPClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
UsesPClause::getAllowedArgsSynonym() {
  return make_pair(PROCEDURE_SYNONYM, VARIABLE_SYNONYM);
}

pair<RefTypeSet, RefTypeSet> UsesPClause::getAllowedArgsRefType() {
  return make_pair(entRefIndexSet, entRefIndexSet);
}