//
// Created by Xingchen Lin on 30/9/22.
//

#include "ModifiesPClause.h"

ModifiesPClause::ModifiesPClause(Ref arg1, Ref arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool ModifiesPClause::operator==(const ModifiesPClause &other) const {
  auto clause = dynamic_cast<const ModifiesPClause *>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

shared_ptr<Table> ModifiesPClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
ModifiesPClause::getAllowedArgsSynonym() {
  return make_pair(PROCEDURE_SYNONYM, VARIABLE_SYNONYM);
}

pair<RefTypeSet, RefTypeSet> ModifiesPClause::getAllowedArgsRefType() {
  return make_pair(entRefIndexSet, entRefIndexSet);
}