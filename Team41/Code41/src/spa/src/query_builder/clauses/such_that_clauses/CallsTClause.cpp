//
// Created by Xingchen Lin on 30/9/22.
//

#include "CallsTClause.h"

CallsTClause::CallsTClause(Ref arg1, Ref arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool CallsTClause::operator==(const CallsTClause &other) const {
  auto clause = dynamic_cast<const CallsTClause *>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

shared_ptr<Table> CallsTClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
CallsTClause::getAllowedArgsSynonym() {
  return make_pair(PROCEDURE_SYNONYM, PROCEDURE_SYNONYM);
}

pair<RefTypeSet, RefTypeSet> CallsTClause::getAllowedArgsRefType() {
  return make_pair(entRefIndexSet, entRefIndexSet);
}