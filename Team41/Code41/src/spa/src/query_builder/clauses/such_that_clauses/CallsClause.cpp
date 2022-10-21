//
// Created by Xingchen Lin on 30/9/22.
//

#include "CallsClause.h"

CallsClause::CallsClause(Ref arg1, Ref arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool CallsClause::operator==(const CallsClause &other) const {
  auto clause = dynamic_cast<const CallsClause *>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table CallsClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
CallsClause::getAllowedArgsSynonym() {
  return make_pair(PROCEDURE_SYNONYM, PROCEDURE_SYNONYM);
}

pair<RefTypeSet, RefTypeSet> CallsClause::getAllowedArgsRefType() {
  return make_pair(entRefIndexSet, entRefIndexSet);
}